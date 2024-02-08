/*
 * Rad Pro
 * STM32 real-time clock
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rcc.h>

#if defined(STM32F0) || defined(STM32F1)

#include <libopencm3/stm32/rtc.h>

#endif

#include "../events.h"
#include "../rtc.h"

#if defined(STM32F0) || defined(STM32G0)

#if defined(STM32G0)

// Missing libopencm3 definitions and functions

#define RCC_RTCAPB _REG_BIT(RCC_APBENR1_OFFSET, 10)

#undef RCC_BDCR_RTCSEL_LSE
#define RCC_BDCR_RTCSEL_LSE (1 << RCC_BDCR_RTCSEL_SHIFT);

#define RTC_TR MMIO32(RTC_BASE + 0x00)
#define RTC_TR_HU_SHIFT 16
#define RTC_TR_MNU_SHIFT 8
#define RTC_TR_SU_SHIFT 0

#define RTC_DR MMIO32(RTC_BASE + 0x04)
#define RTC_DR_YU_SHIFT 16
#define RTC_DR_MU_SHIFT 8
#define RTC_DR_DU_SHIFT 0

#define RTC_SSR MMIO32(RTC_BASE + 0x08)

#define RTC_ICSR MMIO32(RTC_BASE + 0x0c)
#define RTC_ICSR_RSF (1 << 5)
#define RTC_ICSR_INITF (1 << 6)
#define RTC_ICSR_INIT (1 << 7)

#define RTC_WPR MMIO32(RTC_BASE + 0x24)

void rtc_unlock(void);
void rtc_lock(void);
void rtc_wait_for_synchro(void);

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
    rtc_unlock();

    RTC_ICSR &= ~(RTC_ICSR_RSF);
    while (!(RTC_ICSR & RTC_ICSR_RSF))
        ;

    rtc_lock();
}

static void rtc_set_init_flag(void)
{
    RTC_ICSR |= RTC_ICSR_INIT;
}

static void rtc_clear_init_flag(void)
{
    RTC_ICSR &= ~RTC_ICSR_INIT;
}

static void rtc_wait_for_init(void)
{
    while (!(RTC_ICSR & RTC_ICSR_INITF))
        ;
}

#else

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

#endif

// Commmon

void initRTCHardware(void)
{
    // RTC

    rcc_periph_clock_enable(RCC_PWR);
#if defined(RCC_RTCAPB)
    rcc_periph_clock_enable(RCC_RTCAPB);
#endif

    // pwr_disable_backup_domain_write_protect();
#if defined(STM32F0)
    PWR_CR |= PWR_CR_DBP;
#elif defined(STM32G0)
    PWR_CR1 |= PWR_CR1_DBP;
#endif

    // rcc_osc_on(RCC_LSE);
    RCC_BDCR |= RCC_BDCR_LSEON;
    // rcc_wait_for_osc_ready(RCC_LSE);
    while (!(RCC_BDCR & RCC_BDCR_LSERDY))
        ;

    RCC_BDCR |= RCC_BDCR_RTCSEL_LSE; // rcc_set_rtc_clock_source(RCC_LSE);
    RCC_BDCR |= RCC_BDCR_RTCEN;      // rcc_enable_rtc_clock();

    rtc_wait_for_synchro();
}

static uint32_t convertToBCD(uint32_t value)
{
    return ((value / 10) << 4) | (value % 10);
}

static uint32_t convertFromBCD(uint32_t value)
{
    return 10 * ((value >> 4) & 0xf) + (value & 0xf);
}

void setRTCTime(uint32_t value)
{
    RTCDateTime dateTime;
    getDateTimeFromTime(value, &dateTime);

    // Set RTC dateTime

    uint32_t dr =
        convertToBCD(dateTime.year % 100) << RTC_DR_YU_SHIFT |
        convertToBCD(dateTime.month) << RTC_DR_MU_SHIFT |
        convertToBCD(dateTime.day) << RTC_DR_DU_SHIFT;
    uint32_t tr =
        convertToBCD(dateTime.hour) << RTC_TR_HU_SHIFT |
        convertToBCD(dateTime.minute) << RTC_TR_MNU_SHIFT |
        convertToBCD(dateTime.second) << RTC_TR_SU_SHIFT;

    rtc_unlock();

    rtc_set_init_flag();
    rtc_wait_for_init();

    RTC_TR = tr;
    RTC_DR = dr;

    rtc_clear_init_flag();

    rtc_lock();

    rtc_wait_for_synchro();
}

uint32_t getRTCTime(void)
{
    RTCDateTime dateTime;

    // Wait if RTC is about to roll over

    while (RTC_SSR >= 254)
        sleep(1);

    uint32_t tr = RTC_TR;
    uint32_t dr = RTC_DR;

    dateTime.year = 2000 + convertFromBCD(dr >> RTC_DR_YU_SHIFT);
    dateTime.month = convertFromBCD((dr >> RTC_DR_MU_SHIFT) & 0x1f);
    dateTime.day = convertFromBCD(dr >> RTC_DR_DU_SHIFT);
    dateTime.hour = convertFromBCD(tr >> RTC_TR_HU_SHIFT);
    dateTime.minute = convertFromBCD(tr >> RTC_TR_MNU_SHIFT);
    dateTime.second = convertFromBCD(tr >> RTC_TR_SU_SHIFT);

    return getTimeFromDateTime(&dateTime);
}

#elif defined(STM32F1)

void initRTCHardware(void)
{
    rcc_periph_clock_enable(RCC_PWR);
    rcc_periph_clock_enable(RCC_BKP);

    PWR_CR |= PWR_CR_DBP; // pwr_disable_backup_domain_write_protect();

    // rtc_auto_awake(RCC_LSE, 32767);
    bool clockSetup = !(RCC_BDCR & RCC_BDCR_RTCEN);
    if (clockSetup)
    {
        // Enable LSE clock
        RCC_BDCR |= RCC_BDCR_LSEON;
        while (!(RCC_BDCR & RCC_BDCR_LSERDY))
            ;

        // Select LSE as RTC clock source
        RCC_BDCR &= ~(0b11 << 8);
        RCC_BDCR |= (0b01 << 8);

        // Enable RTC clock
        RCC_BDCR |= RCC_BDCR_RTCEN;
    }

    // Sync
    RTC_CRL &= ~RTC_CRL_RSF;
    while (!(RTC_CRL & RTC_CRL_RSF))
        ;

    if (clockSetup)
        rtc_set_counter_val(RTC_TIME_START);
}

void setRTCTime(uint32_t value)
{
    rtc_set_counter_val(value);
}

uint32_t getRTCTime(void)
{
    // Wait if RTC is about to roll over

    while (RTC_DIVL > (32767 * 990 / 1000))
        sleep(1);

    return rtc_get_counter_val();
}

#endif

#endif
