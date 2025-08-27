/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

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
#include "rng.h"
#include "settings.h"
#include "sound.h"
#include "tube.h"
#include "vibration.h"

#define SOUND_PULSE_CLICKS_TICKS ((uint32_t)(0.001 * SYSTICK_FREQUENCY))
#define SOUND_PULSE_CHIRPS_TICKS ((uint32_t)(0.004 * SYSTICK_FREQUENCY))
#define SOUND_PULSE_BEEPS_TICKS ((uint32_t)(0.015 * SYSTICK_FREQUENCY))
#define PULSE_VIBRATION_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_DISPLAY_FLASH_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define ALERT_SOUND_SHORT_TICKS ((uint32_t)(0.250 * SYSTICK_FREQUENCY))
#define ALERT_SOUND_LONG_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))
#define ALERT_VIBRATION_TICKS ((uint32_t)(0.250 * SYSTICK_FREQUENCY))
#define ALERT_DISPLAY_FLASH_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))

#define POWERON_TEST_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

#define LOCKMODE_BACKLIGHT_TICKS (10 * SYSTICK_FREQUENCY)

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

volatile uint32_t eventsTick;

static struct
{
    bool pulseThresholdExceeded;

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

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    bool pulseLEDEnabled;
    volatile int32_t pulseLEDTimer;
#endif

#if defined(VIBRATION)
    volatile int32_t vibrationTimer;
#endif

    int32_t keyboardTimer;

    int32_t heartbeatTimer;
    volatile uint32_t heartbeatUpdate;
    uint32_t lastHeartbeatUpdate;
} events;

static Menu pulsesMenu;
static Menu pulsesThresholdMenu;

static const int32_t displayTimerValues[] = {
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
static const uint32_t pulseSoundTicks[] = {
    SOUND_PULSE_CLICKS_TICKS,
    SOUND_PULSE_CHIRPS_TICKS,
    SOUND_PULSE_BEEPS_TICKS,
};

static const uint32_t alertSoundTicks[] = {
    ALERT_SOUND_SHORT_TICKS,
    ALERT_SOUND_LONG_TICKS,
};
#endif

static void syncTick(void);

void initEvents(void)
{
    events.keyboardTimer = KEY_TICKS;

    events.heartbeatTimer = SYSTICK_FREQUENCY;

    initEventsHardware();
}

void resetEvents(void)
{
    syncTick();

    events.deadTimeCount = PULSE_MEASUREMENT_FREQUENCY;

    events.heartbeatTimer = SYSTICK_FREQUENCY;
    events.heartbeatUpdate = 0;
    events.lastHeartbeatUpdate = 0;

    selectMenuItem(&pulsesMenu,
                   0,
                   0);
    selectMenuItem(&pulsesThresholdMenu,
                   settings.pulseThreshold,
                   RATE_NUM);
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

uint32_t getTick(void)
{
    return eventsTick;
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

    // Heartbeat
    if (updateTimer(&events.heartbeatTimer) == TIMER_ELAPSED)
    {
        events.heartbeatTimer = SYSTICK_FREQUENCY;
        events.heartbeatUpdate++;

        onMeasurementHeartbeat();
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

    // Sound enable
#if defined(SOUND_EN)
    onSoundTick();
#endif

    // Voice
#if defined(VOICE)
    onVoiceTick();
#endif

    // Vibration
#if defined(VIBRATION)
    if (updateTimer(&events.vibrationTimer) == TIMER_ELAPSED)
        setVibration(false);
#endif

    // Pulse LED
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
#endif
}

static void syncTick(void)
{
#if !defined(SIMULATOR)
    sleep(1);
#endif
}

// Events

void dispatchEvents(void)
{
    dispatchCommEvents();

    uint32_t heartbeatUpdate = events.heartbeatUpdate;
    if (events.lastHeartbeatUpdate != heartbeatUpdate)
    {
        events.lastHeartbeatUpdate = heartbeatUpdate;

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

static void setBacklightTimer(int32_t ticks)
{
    if ((ticks == -1) ||
        ((events.backlightTimer != -1) &&
         (ticks > events.backlightTimer)))
    {
        if (ticks != 1)
            setBacklight(true);

        events.backlightTimer = ticks;
    }
}

#if defined(BUZZER)
static void setBuzzerTimer(int32_t ticks, int32_t noiseTicks)
{
    if (ticks > events.buzzerTimer)
    {
        events.buzzerTimer = ticks;
        events.buzzerNoiseTimer = noiseTicks;
    }
}
#endif

#if defined(PULSE_LED)
void enablePulseLED(bool value)
{
    events.pulseLEDEnabled = value;
}

static void setPulseLEDTimer(int32_t ticks)
{
    if (ticks > events.pulseLEDTimer)
    {
        setPulseLED(true);

        events.pulseLEDTimer = ticks;
    }
}

void cancelPulseLED(void)
{
    syncTick();

    events.pulseLEDTimer = 0;
}
#endif

#if defined(VIBRATION)
static void setVibrationTimer(int32_t ticks)
{
    if (ticks > events.vibrationTimer)
    {
        setVibration(true);

        events.vibrationTimer = ticks;
    }
}
#endif

void requestBacklightTrigger(void)
{
    if (isInLockMode())
        events.requestedBacklightTimer = LOCKMODE_BACKLIGHT_TICKS;
    else
        events.requestedBacklightTimer = displayTimerValues[settings.displaySleep];
    events.requestedBacklightReset = true;
}

static void requestBacklightAlert(void)
{
    if ((events.requestedBacklightTimer >= 0) &&
        events.requestedBacklightTimer < ALERT_DISPLAY_FLASH_TICKS)
        events.requestedBacklightTimer = ALERT_DISPLAY_FLASH_TICKS;
}

bool isBacklightTriggerRequested(void)
{
    return events.requestedBacklightTimer != 0;
}

void triggerBacklight(void)
{
    syncTick();

    if (events.requestedBacklightReset)
        events.backlightTimer = 0;

    setBacklightTimer(events.requestedBacklightTimer);

    events.requestedBacklightTimer = 0;
    events.requestedBacklightReset = false;
}

void cancelBacklight(void)
{
    syncTick();

    events.backlightTimer = 0;

    setBacklight(false);
}

bool isBacklightActive(void)
{
    return events.backlightTimer != 0;
}

void setPulseThresholdExceeded(bool value)
{
    events.pulseThresholdExceeded = value;
}

bool isPulseThresholdExceeded(void)
{
    return events.pulseThresholdExceeded;
}

void triggerPulse(void)
{
    if (events.pulseThresholdExceeded)
    {
#if defined(BUZZER)
        if (settings.pulseSound)
            setBuzzerTimer(pulseSoundTicks[settings.soundPulseType] + 1,
                           events.buzzerTimer + 1);
#endif

#if defined(VIBRATION)
        if (settings.pulseVibration)
            setVibrationTimer(PULSE_VIBRATION_TICKS);
#endif

#if defined(PULSE_LED)
        if (events.pulseLEDEnabled)
            setPulseLEDTimer(PULSE_LED_TICKS);
#endif

        if (settings.pulseDisplayFlash && isDisplayEnabled())
            setBacklightTimer(PULSE_DISPLAY_FLASH_TICKS);
    }
}

void triggerAlert(void)
{
    syncTick();

#if defined(VOICE)
    playVoiceAlert();
#endif

#if defined(BUZZER)
    if (settings.alertSound)
        setBuzzerTimer(alertSoundTicks[settings.soundAlertStyle], 1);
#endif

#if defined(VIBRATION)
    if (settings.alertVibration)
        setVibrationTimer(ALERT_VIBRATION_TICKS);
#endif

    if (settings.alertDisplayFlash)
        requestBacklightAlert();
}

void triggerVibration(void)
{
#if defined(VIBRATION)
    syncTick();

    setVibrationTimer(POWERON_TEST_TICKS);
#endif
}

void togglePulseSound(void)
{
#if defined(BUZZER) || defined(SOUND_EN)
    settings.pulseSound = !settings.pulseSound;
#endif
}

// Menus

void onPulsesSubMenuBack(void);

// Pulse indication menu

enum
{
#if defined(BUZZER) || defined(SOUND_EN)
    PULSEINDICATION_MENU_SOUND,
#endif
#if defined(VIBRATION)
    PULSEINDICATION_MENU_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    PULSEINDICATION_MENU_PULSE_LED,
#endif
    PULSEINDICATION_MENU_DISPLAY_FLASH,
};

static cstring pulseIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(SOUND_EN)
    STRING_SOUND,
#endif
#if defined(VIBRATION)
    STRING_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    STRING_PULSE_LED,
#endif
    STRING_DISPLAY_FLASH,
    NULL,
};

static const char *onPulseIndicationMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BUZZER) || defined(SOUND_EN)
    case PULSEINDICATION_MENU_SOUND:
        *menuStyle = settings.pulseSound;

        break;
#endif

#if defined(VIBRATION)
    case PULSEINDICATION_MENU_VIBRATION:
        *menuStyle = settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    case PULSEINDICATION_MENU_PULSE_LED:
        *menuStyle = settings.pulseLED;

        break;
#endif

    case PULSEINDICATION_MENU_DISPLAY_FLASH:
        *menuStyle = settings.pulseDisplayFlash;

        break;
    }

    return getString(pulseIndicationMenuOptions[index]);
}

static void onPulseIndicationMenuSelect(uint32_t index)
{
    switch (index)
    {
#if defined(BUZZER) || defined(SOUND_EN)
    case PULSEINDICATION_MENU_SOUND:
        settings.pulseSound = !settings.pulseSound;

#if defined(SOUND_EN)
        updateSound(true);
#endif

        break;
#endif

#if defined(VIBRATION)
    case PULSEINDICATION_MENU_VIBRATION:
        settings.pulseVibration = !settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    case PULSEINDICATION_MENU_PULSE_LED:
        settings.pulseLED = !settings.pulseLED;

        updateLED(true);

        break;
#endif

    case PULSEINDICATION_MENU_DISPLAY_FLASH:
        settings.pulseDisplayFlash = !settings.pulseDisplayFlash;

        break;
    }
}

static MenuState pulseIndicationMenuState;

static Menu pulseIndicationMenu = {
    STRING_INDICATION,
    &pulseIndicationMenuState,
    onPulseIndicationMenuGetOption,
    onPulseIndicationMenuSelect,
    onPulsesSubMenuBack,
};

View pulseIndicationMenuView = {
    onMenuEvent,
    &pulseIndicationMenu,
};

// Pulse threshold menu

static const char *onPulseThresholdMenuGetOption(uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseThreshold);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < RATE_NUM)
        return buildRateAlertMenuOption(index);
    else
        return NULL;
}

static void onPulseThresholdMenuSelect(uint32_t index)
{
    settings.pulseThreshold = index;

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    updateLED(true);
#endif
}

static MenuState pulseThresholdMenuState;

static Menu pulsesThresholdMenu = {
    STRING_THRESHOLD,
    &pulseThresholdMenuState,
    onPulseThresholdMenuGetOption,
    onPulseThresholdMenuSelect,
    onPulsesSubMenuBack,
};

View pulseThresholdMenuView = {
    onMenuEvent,
    &pulsesThresholdMenu,
};

// Pulses menu

static SubMenuOption pulsesMenuOptions[] = {
    {STRING_INDICATION, &pulseIndicationMenuView},
    {STRING_THRESHOLD, &pulseThresholdMenuView},
    {NULL},
};

static const char *onPulsesMenuGetOption(uint32_t index,
                                         MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(pulsesMenuOptions[index].title);
}

static void onPulsesMenuSelect(uint32_t index)
{
    setView(pulsesMenuOptions[index].view);
}

static MenuState pulsesMenuState;

static Menu pulsesMenu = {
    STRING_PULSES,
    &pulsesMenuState,
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    onSettingsSubMenuBack,
};

View pulsesMenuView = {
    onMenuEvent,
    &pulsesMenu,
};

void onPulsesSubMenuBack(void)
{
    setView(&pulsesMenuView);
}
