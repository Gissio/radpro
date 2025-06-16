/*
 * Rad Pro
 * Simulator system
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../system.h"
#include "../voice.h"

void initSystem(void)
{
}

void getDeviceId(char *str)
{
    strcpy(str, "b5706d937087f975b5812810");
}

void startBootloader(void)
{
}

// Pulse control

void initPulseControl(void)
{
}

void updatePulseControl(void)
{
}

// Voice

void initVoice(void)
{
}

void updateVoice(void)
{
}

void playVoiceInstantaneousRate(void)
{
}

void playVoiceAverageRate(void)
{
}

void playVoiceCumulativeDose(void)
{
}

void playVoiceAlarm(void)
{
}

void playVoiceTest(void)
{
}

#endif
