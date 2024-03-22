/*
 * Rad Pro
 * STM32 real-time clock
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../events.h"
#include "../rtc.h"

#include "device.h"

void initRTCController(void)
{
    syncTimerThread();

    rcc_enable_rtc();
    rtc_disable_backup_domain_write_protection();

    bool rtcEnabled = rtc_is_enabled();
    if (!rtcEnabled)
    {
        rtc_enable_lse();
        while (!rtc_is_lse_ready())
            sleep(1);

        rtc_enable();
        sleep(1);

        setDeviceTime(RTC_TIME_INIT);
    }
}

#if defined(STM32F0) || defined(STM32G0)

static uint32_t convertToBCD(uint32_t value)
{
    return ((value / 10) << 4) | (value % 10);
}

static uint32_t decodeBCDByte(uint8_t value)
{
    return 10 * ((value >> 4) & 0xf) + (value & 0xf);
}

void setDeviceTime(uint32_t value)
{
    RTCDateTime dateTime;
    getDateTimeFromTime(value, &dateTime);

    // Set RTC dateTime
    uint32_t dr =
        convertToBCD(dateTime.year % 100) << RTC_DR_YU_Pos |
        convertToBCD(dateTime.month) << RTC_DR_MU_Pos |
        convertToBCD(dateTime.day) << RTC_DR_DU_Pos;
    uint32_t tr =
        convertToBCD(dateTime.hour) << RTC_TR_HU_Pos |
        convertToBCD(dateTime.minute) << RTC_TR_MNU_Pos |
        convertToBCD(dateTime.second) << RTC_TR_SU_Pos;

    rtc_enter_configuration_mode();
    rtc_set_date_time(dr, tr);
    rtc_leave_configuration_mode();
}

uint32_t getDeviceTime(void)
{
    RTCDateTime dateTime;

    // Wait if RTC is about to roll over
    while (!rtc_is_read_safe())
        sleep(1);

    uint32_t tr;
    uint32_t dr;
    rtc_get_date_time(&dr, &tr);

    dateTime.year = 2000 + decodeBCDByte(dr >> RTC_DR_YU_Pos);
    dateTime.month = decodeBCDByte((dr >> RTC_DR_MU_Pos) & 0x1f);
    dateTime.day = decodeBCDByte(dr >> RTC_DR_DU_Pos);
    dateTime.hour = decodeBCDByte(tr >> RTC_TR_HU_Pos);
    dateTime.minute = decodeBCDByte(tr >> RTC_TR_MNU_Pos);
    dateTime.second = decodeBCDByte(tr >> RTC_TR_SU_Pos);

    return getTimeFromDateTime(&dateTime);
}

#elif defined(STM32F1)

void setDeviceTime(uint32_t value)
{
    rtc_enter_configuration_mode();
    rtc_set_prescaler_factor(LSE_FREQUENCY);
    rtc_set_count(value);
    rtc_leave_configuration_mode();
}

uint32_t getDeviceTime(void)
{
    while (!rtc_is_read_safe())
        sleep(1);

    return rtc_get_count();
}

#endif

#endif
