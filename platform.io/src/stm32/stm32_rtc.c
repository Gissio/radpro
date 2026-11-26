/*
 * Rad Pro
 * STM32 real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/rtc.h"
#include "../system/events.h"
#include "../stm32/device.h"

// Datasheets give typical LSE startup times of 0.2-2.75 s at 25 C, rising to
// 10 s at -10 C (STM32F103 data)
#define RTC_LSE_STARTUP_TIMEOUT 10000 // [ms]

static bool rtcInitialized;
static bool rtcEnabled;

void initRTC(void)
{
    // Called on every power-on: probe the LSE only once
    if (rtcInitialized)
        return;

    rtcInitialized = true;

    rcc_enable_rtc();
    rtc_disable_backup_domain_write_protection();

    // The RTC clock source can only be changed through a backup domain reset.
    // A source other than LSE (LSI or HSE, e.g. left by a previous firmware)
    // would not clock the RTC after reset and hang synchronization.
    if (rtc_get_clock_source() > RCC_BDCR_RTCSEL_LSE)
        rtc_reset_backup_domain();

    // Without a running LSE the RTC is left disabled instead of hanging boot
    rtc_enable_lse();
    for (uint32_t i = RTC_LSE_STARTUP_TIMEOUT; !rtc_is_lse_ready(); i--)
    {
        if (!i)
        {
            rtc_disable_lse();

            return;
        }

        syncTick();
    }

    // Selects LSE and enables the RTC (no-op if already running)
    rtc_enable();
    rtc_wait_for_synchronization();

#if defined(STM32F1)
    // The prescaler resets to 0x8000 (divide by 32769). Rewriting the
    // same value does not disturb a running counter.
    rtc_enter_configuration_mode();
    rtc_set_prescaler_factor(LSE_FREQUENCY);
    rtc_leave_configuration_mode();
#endif

    rtcEnabled = true;
}

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#define RTC_TIME_2000 946684800U  // 2000-01-01 00:00:00 UTC
#define RTC_TIME_2100 4102444800U // 2100-01-01 00:00:00 UTC

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
    // Hardware stores a two-digit year (2000-01-01 ... 2099-12-31)
    if (!rtcEnabled ||
        ((value - RTC_TIME_2000) >= (RTC_TIME_2100 - RTC_TIME_2000)))
        return false;

    RTCDateTime dateTime;
    getDateTimeFromTime(value, &dateTime);

    // Weekday: 1 = Monday ... 7 = Sunday (0 is forbidden); 1970-01-01 was a Thursday
    uint32_t weekday = (value / 86400 + 3) % 7 + 1;

    // Set RTC dateTime
    uint32_t dr = encodeBCD(dateTime.year - 2000) << RTC_DR_YU_Pos |
                  weekday << RTC_DR_WDU_Pos |
                  encodeBCD(dateTime.month) << RTC_DR_MU_Pos |
                  encodeBCD(dateTime.day) << RTC_DR_DU_Pos;
    uint32_t tr = encodeBCD(dateTime.hour) << RTC_TR_HU_Pos |
                  encodeBCD(dateTime.minute) << RTC_TR_MNU_Pos |
                  encodeBCD(dateTime.second) << RTC_TR_SU_Pos;

    rtc_enter_configuration_mode();
    rtc_set_date_time(dr, tr);
    rtc_leave_configuration_mode();
    rtc_wait_for_synchronization();

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
    if (!rtcEnabled)
        return 0;

    // Read TR before DR: reading TR freezes DR until DR is read
    uint32_t tr;
    uint32_t dr;
    rtc_get_date_time(&dr, &tr);

    return tr ^ dr;
}

#elif defined(STM32F1)

bool setDeviceTime(uint32_t value)
{
    if (!rtcEnabled)
        return false;

    rtc_enter_configuration_mode();
    rtc_set_count(value);
    rtc_leave_configuration_mode();

    return true;
}

uint32_t getDeviceTime(void)
{
    return getDeviceTimeFast();
}

uint32_t getDeviceTimeFast(void)
{
    if (!rtcEnabled)
        return 0;

    return rtc_get_count();
}

#endif

#endif
