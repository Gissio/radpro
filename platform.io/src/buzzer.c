/*
 * Rad Pro
 * Buzzer
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"

void playSystemAlert(void)
{
    for (uint32_t i = 0; i < 10; i++)
    {
        setBuzzer(true);
        sleep(50);
        setBuzzer(false);
        sleep(50);
    }
}
