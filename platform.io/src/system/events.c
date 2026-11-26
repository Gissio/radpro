/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "../extras/rng.h"
#include "../measurements/datalog.h"
#include "../measurements/pulses.h"
#include "../peripherals/adc.h"
#include "../peripherals/buzzer.h"
#include "../peripherals/comm.h"
#include "../peripherals/display.h"
#include "../peripherals/keyboard.h"
#include "../peripherals/led.h"
#include "../peripherals/pulsesoundenable.h"
#include "../peripherals/vibrator.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/menu.h"

#define PULSE_DISPLAY_FLASH_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define PULSE_SOUND_CLICKS_TICKS ((uint32_t)(0.001 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_CHIRPS_TICKS ((uint32_t)(0.004 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_BEEPS_TICKS ((uint32_t)(0.015 * SYSTICK_FREQUENCY))

#define PULSE_VIBRATION_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define DISPLAY_FLASH_ALERT_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))
#define SOUND_WARNING_SHORT_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define SOUND_WARNING_LONG_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))
#define SOUND_ALARM_SHORT_TICKS ((uint32_t)(0.250 * SYSTICK_FREQUENCY))
#define SOUND_ALARM_LONG_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))

#define INFO_VIBRATION_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

#define LOCKMODE_BACKLIGHT_TICKS (10 * SYSTICK_FREQUENCY)

static struct
{
    volatile int32_t backlightTimer;
    int32_t requestedBacklightTimer;
    bool requestedBacklightAlert;
    volatile bool displayAwake;

#if defined(BUZZER)
    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;
#endif

#if defined(PULSE_LED)
    volatile LEDMode ledMode;
    volatile int32_t pulseLEDTimer;
#endif

#if defined(VIBRATOR)
    volatile int32_t vibrationTimer;
#endif

    int32_t keyboardTimer;

    int32_t heartbeatTimer;
    volatile uint32_t heartbeatCount;
    uint32_t previousHeartbeatCount;
} events;

volatile uint32_t currentTick;

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
// One extra tick: the noise timer turns the buzzer on one tick later
static const uint8_t pulseSoundTicks[] = {
    PULSE_SOUND_CLICKS_TICKS + 1,
    PULSE_SOUND_CHIRPS_TICKS + 1,
    PULSE_SOUND_BEEPS_TICKS + 1,
};
#endif

#if defined(BUZZER) || defined(VIBRATOR)
static const uint16_t alertSoundTicks[][2] = {
    {SOUND_WARNING_SHORT_TICKS, SOUND_WARNING_LONG_TICKS},
    {SOUND_ALARM_SHORT_TICKS, SOUND_ALARM_LONG_TICKS},
};
#endif

// Timer

// Timers: 0 = off, -1 = infinite, otherwise ticks remaining.
// Returns true when the timer elapses.
static bool tickTimer(volatile int32_t *timer)
{
    int32_t value = *timer;
    if (value <= 0)
        return false;

    value--;
    *timer = value;

    return (value == 0);
}

// Extends a timer; returns true if it was extended
static bool extendTimer(volatile int32_t *timer, int32_t ticks)
{
    if (ticks <= *timer)
        return false;

    *timer = ticks;

    return true;
}

// Events

void onTick(void)
{
    // Pulses
    onPulseTick();

    // ADC
#if defined(EMFMETER)
    onADCTick(events.heartbeatTimer);
#endif

    // Heartbeat
    if (tickTimer(&events.heartbeatTimer))
    {
        events.heartbeatTimer = SYSTICK_FREQUENCY;
        events.heartbeatCount++;

        onPulsesHeartbeat();
    }

    // Keyboard
    onKeyboardTick();
    if (tickTimer(&events.keyboardTimer))
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardUpdate();
    }

    // Display
    if (tickTimer(&events.backlightTimer))
    {
        setBacklight(false);

        events.displayAwake = false;
    }

    // Buzzer
#if defined(BUZZER)
#if defined(SIMULATOR)
    onBuzzerTick();
#endif

    // Noise (random bits) while the noise timer runs, then a steady tone
    if (tickTimer(&events.buzzerTimer))
        setBuzzer(false);
    else if (events.buzzerTimer && events.buzzerNoiseTimer)
        setBuzzer(tickTimer(&events.buzzerNoiseTimer) || getRandomBit());
#endif

    // Sound enable
#if defined(PULSESOUND_ENABLE)
    onPulseSoundEnableTick();
#endif

    // Voice
#if defined(VOICE)
    onVoiceTick();
#endif

    // Vibrator
#if defined(VIBRATOR)
    if (tickTimer(&events.vibrationTimer))
        setVibrator(false);
#endif

    // Pulse LED
#if defined(PULSE_LED)
#if defined(LED_MULTIPLEX)
    if (events.ledMode == LEDMODE_MULTIPLEX)
    {
        // Pulse LED 1 of 7 ticks, alert LED the rest
        bool pulsePhase = ((currentTick % 7) == 0);

        setPulseLED(pulsePhase);
        setAlertLED(!pulsePhase);
    }
    else
#endif
    {
        if (tickTimer(&events.pulseLEDTimer))
            setPulseLED(false);
    }
#endif
}

void syncTick(void)
{
    sleep(1);
}

// Events

void updateEvents(void)
{
    updateComm();

    uint32_t heartbeatCount = events.heartbeatCount;
    if (events.previousHeartbeatCount != heartbeatCount)
    {
        events.previousHeartbeatCount = heartbeatCount;

        updateMeasurements();
        updatePowerState();
        updateViewHeartbeat();
    }

    updateDatalog();
    updateRNG();
    updateView();
}

// Heartbeat

void startHeartbeatEvents(void)
{
    syncTick();

    events.heartbeatTimer = SYSTICK_FREQUENCY;
    events.previousHeartbeatCount = events.heartbeatCount;
}

// Keyboard

void startKeyboardEvents(void)
{
    syncTick();

    events.keyboardTimer = KEY_TICKS;
}

// Backlight

static void setBacklightTimer(int32_t ticks)
{
    // Unsigned compare: -1 (infinite) is the largest value
    if ((uint32_t)ticks > (uint32_t)events.backlightTimer)
    {
#if defined(DISPLAY_MONOCHROME)
        // 1 tick: display sleep "always off"
        if (ticks != 1)
#endif
            setBacklight(true);

        events.backlightTimer = ticks;
    }
}

void requestBacklightTrigger(void)
{
    if (isLockModeEnabled())
        events.requestedBacklightTimer = LOCKMODE_BACKLIGHT_TICKS;
    else
        events.requestedBacklightTimer = displayTimerValues[settings.displaySleep];
    events.displayAwake = true;
}

static void requestBacklightAlert(void)
{
    events.requestedBacklightAlert = true;
}

bool isBacklightTriggerRequested(void)
{
    return (events.requestedBacklightTimer != 0) ||
           events.requestedBacklightAlert;
}

void triggerBacklight(void)
{
    syncTick();

    // A backlight request (always non-zero) restarts the timer
    int32_t ticks = events.requestedBacklightTimer;
    if (ticks)
    {
        events.backlightTimer = 0;
        events.displayAwake = true;

        setBacklightTimer(ticks);
    }

    // An alert flash only extends it
    if (events.requestedBacklightAlert)
        setBacklightTimer(DISPLAY_FLASH_ALERT_TICKS);

    events.requestedBacklightTimer = 0;
    events.requestedBacklightAlert = false;
}

void cancelBacklight(void)
{
    syncTick();

    // Drop pending requests, or the next draw would turn the backlight back on
    events.requestedBacklightTimer = 0;
    events.requestedBacklightAlert = false;

    events.backlightTimer = 0;
    events.displayAwake = false;

    setBacklight(false);
}

bool isBacklightActive(void)
{
    return events.backlightTimer != 0;
}

bool isDisplayAwake(void)
{
    return events.displayAwake;
}

// Buzzer

#if defined(BUZZER)
static void setBuzzerTimer(int32_t ticks, int32_t noiseTicks, uint8_t volume)
{
    if (extendTimer(&events.buzzerTimer, ticks))
    {
        events.buzzerNoiseTimer = noiseTicks;

#if defined(BUZZER_VOLUME)
        setBuzzerVolume(volume);
#endif
    }
}

static void setPulseSoundTimer(void)
{
    setBuzzerTimer(pulseSoundTicks[settings.soundPulseStyle],
                   events.buzzerTimer + 1,
                   settings.soundPulseVolume);
}
#endif

// Vibration

#if defined(VIBRATOR)

static void setVibrationTimer(int32_t ticks)
{
    if (extendTimer(&events.vibrationTimer, ticks))
        setVibrator(true);
}

#endif

void triggerVibration(void)
{
#if defined(VIBRATOR)
    syncTick();

    setVibrationTimer(INFO_VIBRATION_TICKS);
#endif
}

void triggerAndWaitForVibration(void)
{
#if defined(VIBRATOR)
    triggerVibration();

    sleep(INFO_VIBRATION_TICKS);
#endif
}

// Pulse LED

#if defined(PULSE_LED)

void setLEDMode(LEDMode mode)
{
    // Called every heartbeat: only act on changes, so that
    // ongoing pulse flashes are not cut short
    if (events.ledMode == mode)
        return;

    syncTick();

    events.ledMode = mode;
    events.pulseLEDTimer = 0;

    setPulseLED(false);
}

static void setPulseLEDTimer(int32_t ticks)
{
    if (extendTimer(&events.pulseLEDTimer, ticks))
        setPulseLED(true);
}

#endif

// Pulse and alert indication

void indicatePulse(void)
{
    // Called from interrupt thread

    if (settings.pulseDisplayFlash && isDisplayEnabled())
        setBacklightTimer(PULSE_DISPLAY_FLASH_TICKS);

#if defined(BUZZER)
    if (settings.pulseSound)
        setPulseSoundTimer();
#endif

#if defined(VIBRATOR)
    if (settings.pulseVibration)
        setVibrationTimer(PULSE_VIBRATION_TICKS);
#endif

#if defined(PULSE_LED)
    if (events.ledMode == LEDMODE_PULSE)
        setPulseLEDTimer(PULSE_LED_TICKS);
#endif
}

void triggerPulseSound(void)
{
#if defined(BUZZER)
    syncTick();

    setPulseSoundTimer();
#endif
}

// measurementAlert: on voice builds, voice alerts may read out the
// active rate or dose alert instead of the alert sound
static void triggerAlertIndication(bool alarm, bool measurementAlert)
{
    syncTick();

    if (settings.alertDisplayFlash)
        requestBacklightAlert();

#if defined(BUZZER) || defined(VIBRATOR)
    uint32_t alertTicks = alertSoundTicks[alarm][settings.soundAlertStyle];
#endif

#if defined(BUZZER)
    if (settings.alertSound)
        setBuzzerTimer(alertTicks, 1, settings.soundAlertVolume);
#endif

#if defined(VOICE)
    triggerVoiceAlert(measurementAlert);
#endif

#if defined(VIBRATOR)
    if (settings.alertVibration)
        setVibrationTimer(alertTicks);
#endif
}

void triggerAlert(bool alarm)
{
    triggerAlertIndication(alarm, true);
}

void triggerNotificationAlert(void)
{
    triggerAlertIndication(true, false);
}
