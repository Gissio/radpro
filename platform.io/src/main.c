/*
 * Rad Pro
 * Main module
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include "devices/adc.h"
#include "devices/comm.h"
#include "devices/display.h"
#include "devices/emf.h"
#include "devices/flash.h"
#include "devices/keyboard.h"
#include "devices/led.h"
#include "devices/sound.h"
#include "devices/tube.h"
#include "devices/vibrator.h"
#include "extras/game.h"
#include "measurements/datalog.h"
#include "system/events.h"
#include "system/power.h"
#include "system/settings.h"
#include "system/system.h"
#include "ui/view.h"

#if defined(SIMULATOR)
bool onSDLTick();

static void simulateFrame(void)
{
    while (onSDLTick())
    {
#if defined(GAME)
        updateGame();
#endif
        updateEvents();
    }
}
#endif

int main(void)
{
    // System initialization
    initSystem();
    initEvents();
    initPower();
    initFlash();
    initSettings();
    initADC();
    initTube();
#if defined(EMFMETER)
    initEMFMeter();
#endif
    initDatalog();
    initComm();
    initKeyboard();
    initDisplay();
    initView();
#if defined(BUZZER) || defined(PULSESOUND_ENABLE) || defined(VOICE)
    initSound();
#endif
#if defined(VIBRATOR)
    initVibrator();
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    initLED();
#endif

    powerOn(true);

    // Main loop
#if defined(SIMULATOR)
#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(simulateFrame, 0, 1);
#else
    while (true)
        simulateFrame();
#endif
#else
    while (true)
    {
        sleep(1);

#if defined(GAME)
        updateGame();
#endif
        updateEvents();
    }
#endif
}
