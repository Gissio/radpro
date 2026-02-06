/*
 * Rad Pro
 * STM32 real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../devices/rtc.h"
#include "../system/events.h"
#include "../stm32/device.h"

static bool rtcEnabled;

void initRTC(void)
{
    rcc_enable_rtc();
    rtc_disable_backup_domain_write_protection();

    if (!rtc_is_enabled())
    {
        rtc_enable_lse();
        while (!rtc_is_lse_ready())
            sleep(1);

        rtc_enable();
        sleep(1);
    }

    rtcEnabled = true;
}

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

static uint32_t encodeBCD(uint32_t value)
{
    return ((value / 10) << 4) | (value % 10);
}

static uint32_t decodeBCD(uint8_t value)
{
    return 10 * ((value >> 4) & 0xf) + (value & 0xf);
}

bool setDeviceTime(uint32_t value)
{
    if (!rtcEnabled)
        return false;

    RTCDateTime dateTime;
    getDateTimeFromTime(value, &dateTime);

    // Set RTC dateTime
    uint32_t dr = encodeBCD(dateTime.year % 100) << RTC_DR_YU_Pos |
                  encodeBCD(dateTime.month) << RTC_DR_MU_Pos |
                  encodeBCD(dateTime.day) << RTC_DR_DU_Pos;
    uint32_t tr = encodeBCD(dateTime.hour) << RTC_TR_HU_Pos |
                  encodeBCD(dateTime.minute) << RTC_TR_MNU_Pos |
                  encodeBCD(dateTime.second) << RTC_TR_SU_Pos;

    rtc_enter_configuration_mode();
    rtc_set_date_time(dr, tr);
    rtc_leave_configuration_mode();

    return true;
}

uint32_t getDeviceTime(void)
{
    if (!rtcEnabled)
        return 0;

    RTCDateTime dateTime;

    uint32_t tr;
    uint32_t dr;
    rtc_get_date_time(&dr, &tr);

    dateTime.year = 2000 + decodeBCD(dr >> RTC_DR_YU_Pos);
    dateTime.month = decodeBCD((dr >> RTC_DR_MU_Pos) & 0x1f);
    dateTime.day = decodeBCD(dr >> RTC_DR_DU_Pos);
    dateTime.hour = decodeBCD(tr >> RTC_TR_HU_Pos);
    dateTime.minute = decodeBCD(tr >> RTC_TR_MNU_Pos);
    dateTime.second = decodeBCD(tr >> RTC_TR_SU_Pos);

    return getTimeFromDateTime(&dateTime);
}

uint32_t getDeviceTimeFast(void)
{
    return RTC->TR ^ RTC->DR;
}

#elif defined(STM32F1)

bool setDeviceTime(uint32_t value)
{
    if (!rtcEnabled)
        return false;

    rtc_enter_configuration_mode();
    rtc_set_prescaler_factor(LSE_FREQUENCY);
    rtc_set_count(value);
    rtc_leave_configuration_mode();

    return true;
}

uint32_t getDeviceTime(void)
{
    if (!rtcEnabled)
        return 0;

    return getDeviceTimeFast();
}

uint32_t getDeviceTimeFast(void)
{
    return rtc_get_count();
}

#endif

#endif
