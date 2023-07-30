/*
 * Rad Pro
 * STM32 RTC interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)

#include <libopencm3/stm32/pwr.h>
#ifndef STM32G0
#include <libopencm3/stm32/rtc.h>
#endif
#include <libopencm3/stm32/rcc.h>

#include "../../events.h"
#include "../../rtc.h"

#if defined(STM32F0) || defined(STM32G0)

#ifdef STM32G070xx

#define RCC_BDCR_RTCSEL (3 << RCC_BDCR_RTCSEL_SHIFT)

#define RTC_TR MMIO32(RTC_BASE + 0x00)
#define RTC_TR_HU_SHIFT 16
#define RTC_TR_MNU_SHIFT 8
#define RTC_TR_SU_SHIFT 0

#define RTC_DR MMIO32(RTC_BASE + 0x04)
#define RTC_DR_YU_SHIFT 16
#define RTC_DR_MU_SHIFT 8
#define RTC_DR_DU_SHIFT 0

#define RTC_SSR MMIO32(RTC_BASE + 0x08)

#define RTC_ISR MMIO32(RTC_BASE + 0x0c)
#define RTC_ISR_INIT (1 << 7)
#define RTC_ISR_INITF (1 << 6)
#define RTC_ISR_RSF (1 << 5)

#define RTC_WPR MMIO32(RTC_BASE + 0x24)

void rcc_set_rtc_clock_source(enum rcc_osc clk);
void rcc_enable_rtc_clock(void);

void rtc_unlock(void);
void rtc_lock(void);
void rtc_wait_for_synchro(void);

void rcc_set_rtc_clock_source(enum rcc_osc clk)
{
    switch (clk)
    {
    case RCC_HSE:
        RCC_BDCR = (RCC_BDCR & ~RCC_BDCR_RTCSEL) | RCC_BDCR_RTCSEL_HSE_DIV32;

        break;

    case RCC_LSE:
        RCC_BDCR = (RCC_BDCR & ~RCC_BDCR_RTCSEL) | RCC_BDCR_RTCSEL_LSE;

        break;

    case RCC_LSI:
        RCC_BDCR = (RCC_BDCR & ~RCC_BDCR_RTCSEL) | RCC_BDCR_RTCSEL_LSI;

        break;

    default:
        break;
    }
}

void rcc_enable_rtc_clock(void)
{
	RCC_BDCR |= RCC_BDCR_RTCEN;
}

void rtc_unlock(void)
{
    RTC_WPR = 0xca;
    RTC_WPR = 0x53;
}

void rtc_lock(void)
{
    RTC_WPR = 0xff;
}

void rtc_wait_for_synchro(void)
{
    /* Unlock RTC registers */
    RTC_WPR = 0xca;
    RTC_WPR = 0x53;

    RTC_ISR &= ~(RTC_ISR_RSF);

    while (!(RTC_ISR & RTC_ISR_RSF))
        ;

    /* disable write protection again */
    RTC_WPR = 0xff;
}

#endif

static void rtc_set_init_flag(void)
{
    RTC_ISR |= RTC_ISR_INIT;
}

static void rtc_clear_init_flag(void)
{
    RTC_ISR &= ~RTC_ISR_INIT;
}

static void rtc_wait_for_init(void)
{
    while (!(RTC_ISR & RTC_ISR_INITF))
        ;
}

void initRTC(void)
{
    rcc_periph_clock_enable(RCC_PWR);

    pwr_disable_backup_domain_write_protect();

    rcc_osc_on(RCC_LSE);
    rcc_wait_for_osc_ready(RCC_LSE);

    rcc_set_rtc_clock_source(RCC_LSE);
    rcc_enable_rtc_clock();

    rtc_wait_for_synchro();
}

static uint32_t convertToBCD(uint32_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

static uint32_t convertFromBCD(uint32_t bcd)
{
    return 10 * ((bcd >> 4) & 0xf) + (bcd & 0xf);
}

void setRTCDateTime(struct RTCDateTime *dateTime)
{
    uint32_t year = dateTime->year;

    uint32_t dr =
        convertToBCD(year % 100) << RTC_DR_YU_SHIFT |
        convertToBCD(dateTime->month) << RTC_DR_MU_SHIFT |
        convertToBCD(dateTime->day) << RTC_DR_DU_SHIFT;
    uint32_t tr =
        convertToBCD(dateTime->hour) << RTC_TR_HU_SHIFT |
        convertToBCD(dateTime->minute) << RTC_TR_MNU_SHIFT |
        convertToBCD(dateTime->second) << RTC_TR_SU_SHIFT;

    rtc_unlock();

    rtc_set_init_flag();
    rtc_wait_for_init();

    RTC_TR = tr;
    RTC_DR = dr;

    rtc_clear_init_flag();

    rtc_lock();
}

void getRTCDateTime(struct RTCDateTime *dateTime)
{
    // Wait 10 ms if RTC is about to roll over
    while (RTC_SSR > (255 * 990 / 1000))
        sleep(1);

    uint32_t tr = RTC_TR;
    uint32_t dr = RTC_DR;

    dateTime->year = 2000 + convertFromBCD(dr >> RTC_DR_YU_SHIFT);
    dateTime->month = convertFromBCD((dr >> RTC_DR_MU_SHIFT) & 0x1f);
    dateTime->day = convertFromBCD(dr >> RTC_DR_DU_SHIFT);
    dateTime->hour = convertFromBCD(tr >> RTC_TR_HU_SHIFT);
    dateTime->minute = convertFromBCD(tr >> RTC_TR_MNU_SHIFT);
    dateTime->second = convertFromBCD(tr >> RTC_TR_SU_SHIFT);
}

uint32_t getRTCTimestamp(void)
{
    struct RTCDateTime dateTime;

    getRTCDateTime(&dateTime);

    return getTimestampFromDateTime(&dateTime);
}

#elif STM32F1

void initRTC(void)
{
    rcc_periph_clock_enable(RCC_PWR);

    pwr_disable_backup_domain_write_protect();

    rtc_auto_awake(RCC_LSE, 32767);

    if (rtc_get_counter_val() < 86400)
        rtc_set_counter_val(RTC_TIMESTAMP_MIN);
}

void setRTCDateTime(struct RTCDateTime *dateTime)
{
    uint32_t timestamp = getTimestampFromDateTime(dateTime);

    rtc_set_counter_val(timestamp);
}

void getRTCDateTime(struct RTCDateTime *dateTime)
{
    uint32_t timestamp = getRTCTimestamp();

    getDateTimeFromTimestamp(timestamp, dateTime);
}

uint32_t getRTCTimestamp(void)
{
    // Wait 10 ms if RTC is about to roll over
    while (RTC_DIVL > (32767 * 990 / 1000))
        sleep(1);

    return rtc_get_counter_val();
}

#endif

#endif
