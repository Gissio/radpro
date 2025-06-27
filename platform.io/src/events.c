/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "buzzer.h"
#include "cmath.h"
#include "comm.h"
#include "cstring.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "game.h"
#include "keyboard.h"
#include "led.h"
#include "measurements.h"
#include "power.h"
#include "pulsecontrol.h"
#include "rng.h"
#include "settings.h"
#include "tube.h"
#include "vibration.h"
#include "voice.h"

#define PULSE_SOUND_CLICKS_TICKS ((uint32_t)(0.001 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_CHIRPS_TICKS ((uint32_t)(0.004 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_BEEPS_TICKS ((uint32_t)(0.015 * SYSTICK_FREQUENCY))
#define PULSE_VIBRATION_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_DISPLAY_FLASH_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define ALARM_TICKS ((uint32_t)(0.200 * SYSTICK_FREQUENCY))

#define POWERON_TEST_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

volatile uint32_t eventsCurrentTick;

static struct
{
    bool pulseThreshold;

    bool lastPulseTimeCountInitialized;
    uint32_t lastPulseTimeCount;
    uint32_t deadTimeCount;

    int32_t requestedBacklightTimer;
    bool requestedBacklightReset;
    volatile int32_t backlightTimer;

#if defined(BUZZER)
    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;
#endif

#if defined(PULSE_LED)
    volatile int32_t pulseLEDTimer;
#endif

#if defined(ALERT_LED)
    volatile int32_t alertLEDTimer;
#endif

#if defined(VIBRATION)
    volatile int32_t vibrationTimer;
#endif

    int32_t keyboardTimer;

    int32_t periodTimer;
    volatile uint32_t periodUpdate;
    uint32_t lastPeriodUpdate;
} events;

#if defined(BUZZER)
static uint32_t getPulsesSoundMenuItem();
#endif

static const Menu pulsesMenu;
#if defined(BUZZER)
static const Menu pulsesSoundMenu;
#endif
static const Menu pulsesThresholdMenu;

static int32_t displayTimerValues[] = {
#if defined(DISPLAY_MONOCHROME)
    1,
#endif
    10 * SYSTICK_FREQUENCY,
    30 * SYSTICK_FREQUENCY,
    60 * SYSTICK_FREQUENCY,
    120 * SYSTICK_FREQUENCY,
    300 * SYSTICK_FREQUENCY,
    -1,
};

#if defined(BUZZER)
static uint32_t pulseSoundTicks[] = {
    0,
    PULSE_SOUND_CLICKS_TICKS,
    PULSE_SOUND_CHIRPS_TICKS,
    PULSE_SOUND_BEEPS_TICKS,
};
#endif

void initEvents(void)
{
    events.keyboardTimer = KEY_TICKS;

    events.periodTimer = SYSTICK_FREQUENCY;

    initEventsHardware();
}

void resetEvents(void)
{
    events.deadTimeCount = PULSE_MEASUREMENT_FREQUENCY;

    events.periodTimer = SYSTICK_FREQUENCY;
    events.periodUpdate = 0;
    events.lastPeriodUpdate = 0;

    selectMenuItem(&pulsesMenu,
                   0,
                   0);
#if defined(BUZZER)
    selectMenuItem(&pulsesSoundMenu,
                   getPulsesSoundMenuItem(),
                   PULSE_SOUND_NUM);
#endif
    selectMenuItem(&pulsesThresholdMenu,
                   settings.pulseThreshold,
                   RATEALARM_NUM);
}

static TimerState updateTimer(volatile int32_t *timer)
{
    int32_t timerValue = *timer;

    if (timerValue == 0)
        return TIMER_OFF;
    else if (timerValue > 0)
    {
        timerValue--;

        *timer = timerValue;

        return (timerValue == 0) ? TIMER_ELAPSED : TIMER_RUNNING;
    }
    else
        return TIMER_RUNNING;
}

void onTick(void)
{
    // Measurement
    uint32_t pulseCount = 0;
    uint32_t pulseTimeCount;

    while (getTubePulseTime(&pulseTimeCount))
    {
        onRNGPulse(pulseTimeCount);

        if (events.lastPulseTimeCountInitialized)
        {
            uint32_t deltaTimeCount = pulseTimeCount - events.lastPulseTimeCount;

            if (deltaTimeCount < events.deadTimeCount)
                events.deadTimeCount = deltaTimeCount;
        }
        else
            events.lastPulseTimeCountInitialized = true;
        events.lastPulseTimeCount = pulseTimeCount;

        pulseCount++;
    }

    onMeasurementTick(pulseCount);

    // Period
    if (updateTimer(&events.periodTimer) == TIMER_ELAPSED)
    {
        events.periodTimer = SYSTICK_FREQUENCY;
        events.periodUpdate++;

        onMeasurementPeriod();
    }

    // Keyboard
    if (updateTimer(&events.keyboardTimer) == TIMER_ELAPSED)
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Display
    if (updateTimer(&events.backlightTimer) == TIMER_ELAPSED)
        setBacklight(false);

    // Pulse control
#if defined(PULSE_CONTROL)
    onPulseControlTick();
#endif

    // Voice
#if defined(VOICE)
    onVoiceTick();
#endif

    // Buzzer
#if defined(BUZZER)
#if defined(SIMULATOR)
    updateBuzzer();
#endif

    switch (updateTimer(&events.buzzerTimer))
    {
    case TIMER_ELAPSED:
        setBuzzer(false);

        break;

    case TIMER_RUNNING:
        switch (updateTimer(&events.buzzerNoiseTimer))
        {
        case TIMER_ELAPSED:
            setBuzzer(true);

            break;

        case TIMER_RUNNING:
            setBuzzer(getRandomBit());

            break;

        default:
            break;
        }

    default:
        break;
    }
#endif

    // Vibration
#if defined(VIBRATION)
    if (updateTimer(&events.vibrationTimer) == TIMER_ELAPSED)
        setVibration(false);
#endif

    // Pulse LED
#if defined(PULSE_LED)
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
#endif

    // Alert LED
#if defined(ALERT_LED)
    if (updateTimer(&events.alertLEDTimer) == TIMER_ELAPSED)
        setAlertLED(false);
#endif
}

uint32_t getTick(void)
{
    return eventsCurrentTick;
}

// Events

void syncTimerThread(void)
{
    sleep(1);
}

void dispatchEvents(void)
{
    sleep(0);

    dispatchCommEvents();

    uint32_t periodUpdate = events.periodUpdate;
    if (events.lastPeriodUpdate != periodUpdate)
    {
        events.lastPeriodUpdate = periodUpdate;

        updateMeasurements();
        updateView();
        updateBattery();
    }

    updateDatalog();
    dispatchViewEvents();
}

// Dead time

float getTubeDeadTime(void)
{
    return events.deadTimeCount * timerCountToSeconds;
}

// Timers

static void setBacklightTimer(int32_t value)
{
    if ((value == -1) ||
        ((events.backlightTimer != -1) &&
         (value > events.backlightTimer)))
    {
        if (value != 1)
            setBacklight(true);

        events.backlightTimer = value;
    }
}

#if defined(BUZZER)
static void setBuzzerTimer(int32_t value, int32_t noiseValue)
{
    if (value > events.buzzerTimer)
    {
        events.buzzerTimer = value;
        events.buzzerNoiseTimer = noiseValue;
    }
}
#endif

#if defined(PULSE_LED)
static void setPulseLEDTimer(int32_t value)
{
    if (value > events.pulseLEDTimer)
    {
        setPulseLED(true);

        events.pulseLEDTimer = value;
    }
}
#endif

#if defined(ALERT_LED)
static void setAlertLEDTimer(int32_t value)
{
    if (value > events.alertLEDTimer)
    {
        setAlertLED(true);

        events.alertLEDTimer = value;
    }
}
#endif

#if defined(VIBRATION)
static void setVibrationTimer(int32_t value)
{
    if (value > events.vibrationTimer)
    {
        setVibration(true);

        events.vibrationTimer = value;
    }
}
#endif

void requestBacklightTrigger(void)
{
    events.requestedBacklightTimer = displayTimerValues[settings.displaySleep];
    events.requestedBacklightReset = true;
}

static void requestBacklightAlarm(void)
{
    if ((events.requestedBacklightTimer >= 0) &&
        events.requestedBacklightTimer < ALARM_TICKS)
        events.requestedBacklightTimer = ALARM_TICKS;
}

bool isBacklightTriggerRequested(void)
{
    return events.requestedBacklightTimer != 0;
}

void triggerBacklight(void)
{
    if (events.requestedBacklightReset)
        events.backlightTimer = 0;

    setBacklightTimer(events.requestedBacklightTimer);

    events.requestedBacklightTimer = 0;
    events.requestedBacklightReset = false;
}

void cancelBacklight(void)
{
    events.backlightTimer = 1;
}

bool isBacklightActive(void)
{
    return events.backlightTimer != 0;
}

void setPulseThreshold(bool value)
{
    events.pulseThreshold = value;
}

bool isPulseThresholdEnabled(void)
{
    return events.pulseThreshold;
}

void triggerPulse(void)
{
    if (!events.pulseThreshold)
    {
#if defined(BUZZER)
        if (settings.pulseSound & PULSE_SOUND_ON_MASK)
            setBuzzerTimer(pulseSoundTicks[settings.pulseSound & PULSE_SOUND_TYPE_MASK] + 1,
                           events.buzzerTimer + 1);
#endif

#if defined(VIBRATION)
        if (settings.pulseVibration)
            setVibrationTimer(PULSE_VIBRATION_TICKS);
#endif

#if defined(PULSE_LED)
        if (settings.pulseLED)
            setPulseLEDTimer(PULSE_LED_TICKS);
#endif

        if (settings.pulseDisplayFlash && isDisplayEnabled())
            setBacklightTimer(PULSE_DISPLAY_FLASH_TICKS);
    }
}

void triggerAlarm(void)
{
    syncTimerThread();

#if defined(VOICE)
    playVoiceAlarm();
#endif

#if defined(BUZZER)
    if (settings.alarmIndication & (1 << ALARMINDICATION_SOUND))
        setBuzzerTimer(ALARM_TICKS, 1);
#endif

#if defined(VIBRATION)
    if (settings.alarmIndication & (1 << ALARMINDICATION_VIBRATION))
        setVibrationTimer(ALARM_TICKS);
#endif

#if defined(ALERT_LED)
    if (settings.alarmIndication & (1 << ALARMINDICATION_PULSE_LED))
        setAlertLEDTimer(ALARM_TICKS);
#elif defined(PULSE_LED)
    if (settings.alarmIndication & (1 << ALARMINDICATION_PULSE_LED))
        setPulseLEDTimer(ALARM_TICKS);
#endif

    if (settings.alarmIndication & (1 << ALARMINDICATION_DISPLAY_FLASH))
        requestBacklightAlarm();
}

void triggerVibration(void)
{
#if defined(VIBRATION)
    syncTimerThread();

    setVibrationTimer(POWERON_TEST_TICKS);
#endif
}

void togglePulseClicks(void)
{
#if defined(PULSE_CONTROL)
    settings.pulseSound = !settings.pulseSound;
#endif
#if defined(BUZZER)
    settings.pulseSound ^= PULSE_SOUND_ON_MASK;
#endif
}

// Pulses sound menu

#if defined(BUZZER)

static const char *const pulsesSoundMenuOptions[] = {
    getString(STRING_OFF),
    getString(STRING_CLICKS),
    getString(STRING_CHIRPS),
    getString(STRING_BEEPS),
    NULL,
};

static uint32_t getPulsesSoundMenuItem()
{
    if (!(settings.pulseSound & PULSE_SOUND_ON_MASK))
        return 0;
    else
        return (settings.pulseSound & PULSE_SOUND_TYPE_MASK);
}

static const char *onPulsesSoundMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == getPulsesSoundMenuItem());

    return pulsesSoundMenuOptions[index];
}

static void onPulsesSoundMenuSelect(const Menu *menu)
{
    if (!menu->state->selectedIndex)
        settings.pulseSound = PULSE_SOUND_OFF_CLICKS;
    else
        settings.pulseSound = menu->state->selectedIndex | PULSE_SOUND_ON_MASK;
}

static MenuState pulsesSoundMenuState;

static const Menu pulsesSoundMenu = {
    getString(STRING_SOUND),
    &pulsesSoundMenuState,
    onPulsesSoundMenuGetOption,
    onPulsesSoundMenuSelect,
    onPulsesSubMenuBack,
};

const View pulsesSoundMenuView = {
    onMenuEvent,
    &pulsesSoundMenu,
};

#endif

// Pulses threshold menu

static const char *onPulseThresholdMenuGetOption(const Menu *menu,
                                                 uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseThreshold);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < RATEALARM_NUM)
        return buildRateAlarmMenuOption(index);
    else
        return NULL;
}

static void onPulseThresholdMenuSelect(const Menu *menu)
{
    settings.pulseThreshold = menu->state->selectedIndex;
}

static MenuState pulseThresholdMenuState;

static const Menu pulsesThresholdMenu = {
    getString(STRING_THRESHOLD),
    &pulseThresholdMenuState,
    onPulseThresholdMenuGetOption,
    onPulseThresholdMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseThresholdMenuView = {
    onMenuEvent,
    &pulsesThresholdMenu,
};

// Pulses menu

enum
{
#if defined(BUZZER) || defined(PULSE_CONTROL)
    PULSES_MENU_OPTIONS_SOUND,
#endif
#if defined(VIBRATION)
    PULSES_MENU_OPTIONS_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(PULSE_CONTROL)
    PULSES_MENU_OPTIONS_PULSE_LED,
#endif
    PULSES_MENU_OPTIONS_DISPLAY_FLASH,
    PULSES_MENU_OPTIONS_PULSE_THRESHOLD,
};

static const char *const pulsesMenuOptions[] = {
#if defined(BUZZER) || defined(PULSE_CONTROL)
    getString(STRING_SOUND),
#endif
#if defined(VIBRATION)
    getString(STRING_VIBRATION),
#endif
#if defined(PULSE_LED) || defined(PULSE_CONTROL)
    getString(STRING_PULSE_LED),
#endif
    getString(STRING_DISPLAY_FLASH),
    getString(STRING_THRESHOLD),
    NULL,
};

static const char *onPulsesMenuGetOption(const Menu *menu,
                                         uint32_t index,
                                         MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(PULSE_CONTROL)
    case PULSES_MENU_OPTIONS_SOUND:
        *menuStyle = settings.pulseSound;

        break;
#endif

#if defined(BUZZER)
    case PULSES_MENU_OPTIONS_SOUND:
#endif
    case PULSES_MENU_OPTIONS_PULSE_THRESHOLD:
        *menuStyle = MENUSTYLE_SUBMENU;

        break;

#if defined(VIBRATION)
    case PULSES_MENU_OPTIONS_VIBRATION:
        *menuStyle = settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_CONTROL)
    case PULSES_MENU_OPTIONS_PULSE_LED:
        *menuStyle = settings.pulseLED;

        break;
#endif

    case PULSES_MENU_OPTIONS_DISPLAY_FLASH:
        *menuStyle = settings.pulseDisplayFlash;

        break;
    }

    return pulsesMenuOptions[index];
}

static void onPulsesMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
#if defined(PULSE_CONTROL)
    case PULSES_MENU_OPTIONS_SOUND:
        settings.pulseSound = !settings.pulseSound;

        updatePulseControl();

        break;
#endif

#if defined(BUZZER)
    case PULSES_MENU_OPTIONS_SOUND:
        setView(&pulsesSoundMenuView);

        break;
#endif

#if defined(VIBRATION)
    case PULSES_MENU_OPTIONS_VIBRATION:
        settings.pulseVibration = !settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_CONTROL)
    case PULSES_MENU_OPTIONS_PULSE_LED:
        settings.pulseLED = !settings.pulseLED;

#if defined(PULSE_CONTROL)
        updatePulseControl();
#endif

        break;
#endif

    case PULSES_MENU_OPTIONS_DISPLAY_FLASH:
        settings.pulseDisplayFlash = !settings.pulseDisplayFlash;

        break;

    case PULSES_MENU_OPTIONS_PULSE_THRESHOLD:
        setView(&pulseThresholdMenuView);

        break;
    }
}

static MenuState pulsesMenuState;

static const Menu pulsesMenu = {
    getString(STRING_PULSES),
    &pulsesMenuState,
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    onSettingsSubMenuBack,
};

const View pulsesMenuView = {
    onMenuEvent,
    &pulsesMenu,
};

void onPulsesSubMenuBack(const Menu *menu)
{
    setView(&pulsesMenuView);
}
