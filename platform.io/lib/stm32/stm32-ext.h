/*
 * STM32 CMSIS extensions
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#ifndef _STM32_EXT_H_
#define _STM32_EXT_H_

#include <stdbool.h>
#include <stdint.h>

#include "stm32.h"

// RCC

#if defined(STM32F1)
#define RCC_CFGR_PLLSRC_HSI_DIV2 0
#define RCC_CFGR_PLLSRC_HSE RCC_CFGR_PLLSRC

#define RCC_CFGR_USBPRE_DIV1 RCC_CFGR_USBPRE_Msk
#define RCC_CFGR_USBPRE_DIV1_5 0
#elif defined(STM32G0)
#define RCC_CR_HSIDIV_DIV1 (0x0 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV2 (0x1 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV4 (0x2 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV8 (0x3 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV16 (0x4 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV32 (0x5 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV64 (0x6 << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV_DIV128 (0x7 << RCC_CR_HSIDIV_Pos)
#endif

__STATIC_INLINE void rcc_enable_crc(void)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)
    set_bits(RCC->AHBENR,
             RCC_AHBENR_CRCEN);
#elif defined(STM32L4)
    set_bits(RCC->AHB1ENR,
             RCC_AHB1ENR_CRCEN);
#endif
}

__STATIC_INLINE void rcc_disable_crc(void)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)
    clear_bits(RCC->AHBENR,
               RCC_AHBENR_CRCEN);
#elif defined(STM32L4)
    clear_bits(RCC->AHB1ENR,
               RCC_AHB1ENR_CRCEN);
#endif
}

#if defined(STM32F1)
__STATIC_INLINE void rcc_enable_afio(void)
{
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_AFIOEN);
}
#endif

__STATIC_INLINE void rcc_enable_adc(const ADC_TypeDef *base)
{
#if defined(STM32F0)
    set_bits(RCC->CR2, RCC_CR2_HSI14ON);
    wait_until_bits_set(RCC->CR2, RCC_CR2_HSI14RDY);

    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_ADCEN);
#elif defined(STM32F1)
    if (base == ADC1)
        set_bits(RCC->APB2ENR,
                 RCC_APB2ENR_ADC1EN);
#if defined(ADC2)
    else if (base == ADC2)
        set_bits(RCC->APB2ENR,
                 RCC_APB2ENR_ADC2EN);
#endif
#elif defined(STM32G0)
    set_bits(RCC->APBENR2,
             RCC_APBENR2_ADCEN);
#elif defined(STM32L4)
    set_bits(RCC->AHB2ENR,
             RCC_AHB2ENR_ADCEN);
#endif
}

__STATIC_INLINE void rcc_enable_tim(const TIM_TypeDef *base)
{
#if defined(STM32F0)
    if (base == TIM1)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    else if (base == TIM2)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
    else if (base == TIM3)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
    else if (base == TIM6)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
    else if (base == TIM14)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
    else if (base == TIM15)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM15EN);
    else if (base == TIM16)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM16EN);
    else if (base == TIM17)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM17EN);
#elif defined(STM32F1)
    if (base == TIM1)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    else if (base == TIM2)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
    else if (base == TIM3)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
    else if (base == TIM4)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
#elif defined(STM32G0)
    if (base == TIM1)
        set_bits(RCC->APBENR2, RCC_APBENR2_TIM1EN);
    else if (base == TIM3)
        set_bits(RCC->APBENR1, RCC_APBENR1_TIM3EN);
    else if (base == TIM6)
        set_bits(RCC->APBENR1, RCC_APBENR1_TIM6EN);
    else if (base == TIM7)
        set_bits(RCC->APBENR1, RCC_APBENR1_TIM7EN);
    else if (base == TIM14)
        set_bits(RCC->APBENR2, RCC_APBENR2_TIM14EN);
    else if (base == TIM15)
        set_bits(RCC->APBENR2, RCC_APBENR2_TIM15EN);
    else if (base == TIM16)
        set_bits(RCC->APBENR2, RCC_APBENR2_TIM16EN);
    else if (base == TIM17)
        set_bits(RCC->APBENR2, RCC_APBENR2_TIM17EN);
#elif defined(STM32L4)
    if (base == TIM1)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    else if (base == TIM2)
        set_bits(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN);
    else if (base == TIM6)
        set_bits(RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN);
    else if (base == TIM7)
        set_bits(RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN);
    else if (base == TIM15)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM15EN);
    else if (base == TIM16)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_TIM16EN);
#endif
}

__STATIC_INLINE void rcc_enable_rtc(void)
{
#if defined(STM32F0)
    set_bits(RCC->APB1ENR,
             RCC_APB1ENR_PWREN);
#elif defined(STM32F1)
    set_bits(RCC->APB1ENR,
             RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN);
#elif defined(STM32G0)
    set_bits(RCC->APBENR1,
             RCC_APBENR1_PWREN | RCC_APBENR1_RTCAPBEN);
#elif defined(STM32L4)
    set_bits(RCC->APB1ENR1,
             RCC_APB1ENR1_PWREN | RCC_APB1ENR1_RTCAPBEN);
#endif
}

__STATIC_INLINE void rcc_enable_usart(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1)
    if (base == USART1)
        set_bits(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
    else if (base == USART2)
        set_bits(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
#elif defined(STM32G0)
    if (base == USART1)
        set_bits(RCC->APBENR2,
                 RCC_APBENR2_USART1EN);
    else if (base == USART2)
        set_bits(RCC->APBENR1,
                 RCC_APBENR1_USART2EN);
#elif defined(STM32L4)
    if (base == USART1)
        set_bits(RCC->APB2ENR,
                 RCC_APB2ENR_USART1EN);
    else if (base == USART2)
        set_bits(RCC->APB1ENR1,
                 RCC_APB1ENR1_USART2EN);
    else if (base == USART3)
        set_bits(RCC->APB1ENR1,
                 RCC_APB1ENR1_USART3EN);
#endif
}

// CRC

__STATIC_INLINE void crc_reset(void)
{
    set_bits(CRC->CR,
             CRC_CR_RESET);
}

__STATIC_INLINE void crc_write(uint32_t data)
{
    CRC->DR = data;
}

__STATIC_INLINE uint32_t crc_read(void)
{
    return CRC->DR;
}

// FLASH

#if !defined(FLASH_ACR_LATENCY_0WS)
#define FLASH_ACR_LATENCY_0WS 0
#define FLASH_ACR_LATENCY_1WS FLASH_ACR_LATENCY_0
#define FLASH_ACR_LATENCY_2WS FLASH_ACR_LATENCY_1
#endif

#if defined(STM32F0)
#define FLASH_PAGE_SIZE 0x400
#define FLASH_WORD_SIZE 0x2

#define FLASH_SR_ERRORS FLASH_SR_WRPERR | \
                            FLASH_SR_PGERR
#define FLASH_SR_CLEAR FLASH_SR_EOP | \
                           FLASH_SR_ERRORS
#elif defined(STM32F1)
#define FLASH_PAGE_SIZE 0x400
#define FLASH_WORD_SIZE 0x2

#define FLASH_SR_ERRORS FLASH_SR_WRPRTERR | \
                            FLASH_SR_PGERR
#define FLASH_SR_CLEAR FLASH_SR_EOP | \
                           FLASH_SR_ERRORS
#elif defined(STM32G0)
#define FLASH_PAGE_SIZE 0x800
#define FLASH_WORD_SIZE 0x8

#define FLASH_KEY1 0x45670123UL
#define FLASH_KEY2 0xCDEF89ABUL

#define FLASH_SR_ERRORS (FLASH_SR_OPTVERR | \
                         FLASH_SR_FASTERR | \
                         FLASH_SR_MISERR |  \
                         FLASH_SR_PGSERR |  \
                         FLASH_SR_SIZERR |  \
                         FLASH_SR_PGAERR |  \
                         FLASH_SR_WRPERR |  \
                         FLASH_SR_PROGERR | \
                         FLASH_SR_OPERR)
#define FLASH_SR_CLEAR (FLASH_SR_ERRORS | \
                        FLASH_SR_EOP)
#elif defined(STM32L4)
#define FLASH_PAGE_SIZE 0x800
#define FLASH_WORD_SIZE 0x8

#define FLASH_KEY1 0x45670123UL
#define FLASH_KEY2 0xCDEF89ABUL

#define FLASH_SR_ERRORS (FLASH_SR_OPTVERR | \
                         FLASH_SR_RDERR |   \
                         FLASH_SR_FASTERR | \
                         FLASH_SR_MISERR |  \
                         FLASH_SR_PGSERR |  \
                         FLASH_SR_SIZERR |  \
                         FLASH_SR_PGAERR |  \
                         FLASH_SR_WRPERR |  \
                         FLASH_SR_PROGERR | \
                         FLASH_SR_OPERR)
#define FLASH_SR_CLEAR (FLASH_SR_ERRORS | \
                        FLASH_SR_EOP)
#endif

__STATIC_INLINE uint32_t flash_get_kb_size(void)
{
    return *((uint32_t *)FLASHSIZE_BASE);
}

__STATIC_INLINE void flash_unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

__STATIC_INLINE void flash_lock(void)
{
    set_bits(FLASH->CR,
             FLASH_CR_LOCK);
}

__STATIC_INLINE void flash_wait_while_busy(void)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    wait_until_bits_clear(FLASH->SR,
                          FLASH_SR_BSY);
#elif defined(STM32G0)
    wait_until_bits_clear(FLASH->SR,
                          FLASH_SR_BSY1);
#endif
}

__STATIC_INLINE void flash_clear_status(void)
{
    FLASH->SR = FLASH_SR_CLEAR;
}

__STATIC_INLINE bool flash_erase_page(uint32_t pageIndex)
{
    flash_wait_while_busy();
    flash_clear_status();

    set_bits(FLASH->CR,
             FLASH_CR_PER);
#if defined(STM32F0) || defined(STM32F1)
    FLASH->AR = FLASH_BASE + pageIndex * FLASH_PAGE_SIZE;
#elif defined(STM32G0) || defined(STM32L4)
    modify_bits(FLASH->CR,
                FLASH_CR_PNB_Msk,
                (pageIndex << FLASH_CR_PNB_Pos));
#endif
    set_bits(FLASH->CR,
             FLASH_CR_STRT);
    flash_wait_while_busy();

    clear_bits(FLASH->CR,
               FLASH_CR_PER);

    return (get_bits(FLASH->SR, FLASH_SR_ERRORS) == 0);
}

__STATIC_INLINE bool flash_program(uint8_t *dest,
                                   uint8_t *source)
{
    flash_wait_while_busy();
    flash_clear_status();

    set_bits(FLASH->CR,
             FLASH_CR_PG);
#if defined(STM32F0) || defined(STM32F1)
    ((uint16_t *)dest)[0] = ((uint16_t *)source)[0];
#elif defined(STM32G0) || defined(STM32L4)
    ((uint32_t *)dest)[0] = ((uint32_t *)source)[0];
    ((uint32_t *)dest)[1] = ((uint32_t *)source)[1];
#endif
    flash_wait_while_busy();

    clear_bits(FLASH->CR,
               FLASH_CR_PG);

    return (get_bits(FLASH->SR, FLASH_SR_ERRORS) == 0);
}

// GPIO

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#define GPIO_MODE_INPUT 0b00
#define GPIO_MODE_OUTPUT 0b01
#define GPIO_MODE_AF 0b10
#define GPIO_MODE_ANALOG 0b11

#define GPIO_OUTPUTTYPE_NONE 0
#define GPIO_OUTPUTTYPE_PUSHPULL 0
#define GPIO_OUTPUTTYPE_OPENDRAIN 1

#define GPIO_OUTPUTSPEED_NONE 0
#define GPIO_OUTPUTSPEED_2MHZ 0b00
#define GPIO_OUTPUTSPEED_10MHZ 0b01
#define GPIO_OUTPUTSPEED_50MHZ 0b11

#define GPIO_PULL_FLOATING 0b00
#define GPIO_PULL_UP 0b01
#define GPIO_PULL_DOWN 0b10

#define GPIO_AF_NONE 0b00
#define GPIO_AF0 0
#define GPIO_AF1 1
#define GPIO_AF2 2
#define GPIO_AF3 3
#define GPIO_AF4 4
#define GPIO_AF5 5
#define GPIO_AF6 6
#define GPIO_AF7 7

__STATIC_INLINE void gpio_setup(GPIO_TypeDef *base,
                                uint8_t pin,
                                uint8_t mode,
                                uint8_t output_type,
                                uint8_t output_speed,
                                uint8_t pull,
                                uint8_t af)
{
    uint32_t shift2 = 2 * pin;
    uint32_t mask2 = 0b11 << shift2;

    modify_bits(base->MODER,
                mask2,
                mode << shift2);

    if ((mode == GPIO_MODE_OUTPUT) ||
        (mode == GPIO_MODE_AF))
    {
        uint32_t shift1 = pin;
        uint32_t mask1 = 1 << shift1;

        modify_bits(base->OTYPER,
                    mask1,
                    output_type << shift1);
        modify_bits(base->OSPEEDR,
                    mask2,
                    output_speed << shift2);
    }

    modify_bits(base->PUPDR,
                mask2,
                pull << shift2);

    if (mode == GPIO_MODE_AF)
    {
        if (pin < 8)
        {
            uint32_t shift4 = 4 * pin;
            modify_bits(base->AFR[0],
                        0b1111 << shift4,
                        af << shift4);
        }
        else
        {
            uint32_t shift4 = 4 * (pin - 8);
            modify_bits(base->AFR[1],
                        0b1111 << shift4,
                        af << shift4);
        }
    }
}

__STATIC_INLINE void gpio_setup_input(GPIO_TypeDef *base,
                                      uint8_t pin,
                                      uint8_t pull)
{
    gpio_setup(base,
               pin,
               GPIO_MODE_INPUT,
               GPIO_OUTPUTTYPE_NONE,
               GPIO_OUTPUTSPEED_NONE,
               pull,
               GPIO_AF_NONE);
}

__STATIC_INLINE void gpio_setup_output(GPIO_TypeDef *base,
                                       uint8_t pin,
                                       uint8_t output_type,
                                       uint8_t output_speed,
                                       uint8_t pull)
{
    gpio_setup(base,
               pin,
               GPIO_MODE_OUTPUT,
               output_type,
               output_speed,
               pull,
               GPIO_AF_NONE);
}

__STATIC_INLINE void gpio_setup_af(GPIO_TypeDef *base,
                                   uint8_t pin,
                                   uint8_t output_type,
                                   uint8_t output_speed,
                                   uint8_t pull,
                                   uint8_t af)
{
    gpio_setup(base,
               pin,
               GPIO_MODE_AF,
               output_type,
               output_speed,
               pull,
               af);
}

__STATIC_INLINE void gpio_setup_analog(GPIO_TypeDef *base,
                                       uint8_t pin,
                                       uint8_t pull)
{
    gpio_setup(base,
               pin,
               GPIO_MODE_ANALOG,
               GPIO_OUTPUTTYPE_NONE,
               GPIO_OUTPUTSPEED_NONE,
               pull,
               GPIO_AF_NONE);
}

#elif defined(STM32F1)

#define GPIO_MODE_INPUT_ANALOG 0b0000
#define GPIO_MODE_INPUT_FLOATING 0b0100
#define GPIO_MODE_INPUT_PULLDOWN 0b01000
#define GPIO_MODE_INPUT_PULLUP 0b11000
#define GPIO_MODE_OUTPUT_2MHZ_PUSHPULL 0b0010
#define GPIO_MODE_OUTPUT_2MHZ_OPENDRAIN 0b0110
#define GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL 0b1010
#define GPIO_MODE_OUTPUT_2MHZ_AF_OPENDRAIN 0b1110
#define GPIO_MODE_OUTPUT_10MHZ_PUSHPULL 0b0001
#define GPIO_MODE_OUTPUT_10MHZ_OPENDRAIN 0b0101
#define GPIO_MODE_OUTPUT_10MHZ_AF_PUSHPULL 0b1001
#define GPIO_MODE_OUTPUT_10MHZ_AF_OPENDRAIN 0b1101
#define GPIO_MODE_OUTPUT_50MHZ_PUSHPULL 0b0011
#define GPIO_MODE_OUTPUT_50MHZ_OPENDRAIN 0b0111
#define GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL 0b1011
#define GPIO_MODE_OUTPUT_50MHZ_AF_OPENDRAIN 0b1111

#define GPIO_MODE_Msk 0b1111

__STATIC_INLINE void gpio_setup(GPIO_TypeDef *base,
                                uint8_t pin,
                                uint8_t mode)
{
    if (mode == GPIO_MODE_INPUT_PULLDOWN)
        clear_bits(base->ODR,
                   get_bitvalue(pin));
    else if (mode == GPIO_MODE_INPUT_PULLUP)
        set_bits(base->ODR,
                 get_bitvalue(pin));

    if (pin < 8)
    {
        uint32_t shift4 = 4 * pin;
        modify_bits(base->CRL,
                    GPIO_MODE_Msk << shift4,
                    (mode & GPIO_MODE_Msk) << shift4);
    }
    else
    {
        uint32_t shift4 = 4 * (pin - 8);
        modify_bits(base->CRH,
                    GPIO_MODE_Msk << shift4,
                    (mode & GPIO_MODE_Msk) << shift4);
    }
}

#endif

__STATIC_INLINE bool gpio_get(const GPIO_TypeDef *base,
                              uint8_t pin)
{
    return get_bits(base->IDR,
                    get_bitvalue(pin));
}

__STATIC_INLINE void gpio_set(GPIO_TypeDef *base,
                              uint8_t pin)
{
    set_bits(base->ODR,
             get_bitvalue(pin));
}

__STATIC_INLINE void gpio_clear(GPIO_TypeDef *base,
                                uint8_t pin)
{
    clear_bits(base->ODR,
               get_bitvalue(pin));
}

__STATIC_INLINE void gpio_modify(GPIO_TypeDef *base,
                                 uint8_t pin,
                                 bool value)
{
    if (value)
        gpio_set(base, pin);
    else
        gpio_clear(base, pin);
}

// NVIC

__STATIC_INLINE void NVIC_DisableAllIRQs(void)
{
    for (uint8_t i = 0;
         i < sizeof(NVIC->ICER) / sizeof(uint32_t);
         i++)
    {
        NVIC->ICER[i] = 0xffffffff;
        NVIC->ICPR[i] = 0xffffffff;
    }
}

// EXTI

#if defined(STM32F0) || defined(STM32L4)
#define STM32EXT_EXTI SYSCFG
#elif defined(STM32F1)
#define STM32EXT_EXTI AFIO
#elif defined(STM32G0)
#define STM32EXT_EXTI EXTI
#endif

__STATIC_INLINE void exti_setup(const GPIO_TypeDef *base,
                                uint8_t pin,
                                bool rising_trigger,
                                bool falling_trigger)
{
    uint32_t shift4 = 4 * (pin & 0b11);

    if (base == GPIOA)
        modify_bits(STM32EXT_EXTI->EXTICR[pin / 4],
                    0b1111 << shift4,
                    0 << shift4);
    else if (base == GPIOB)
        modify_bits(STM32EXT_EXTI->EXTICR[pin / 4],
                    0b1111 << shift4,
                    1 << shift4);
    else if (base == GPIOC)
        modify_bits(STM32EXT_EXTI->EXTICR[pin / 4],
                    0b1111 << shift4,
                    2 << shift4);
    else if (base == GPIOD)
        modify_bits(STM32EXT_EXTI->EXTICR[pin / 4],
                    0b1111 << shift4,
                    3 << shift4);
#if defined(GPIOF)
    else if (base == GPIOF)
        modify_bits(STM32EXT_EXTI->EXTICR[pin / 4],
                    0b1111 << shift4,
                    5 << shift4);
#endif

#if defined(STM32F0) || defined(STM32F1)
    modify_bits(EXTI->RTSR,
                1 << pin,
                rising_trigger << pin);
    modify_bits(EXTI->FTSR,
                1 << pin,
                falling_trigger << pin);
#elif defined(STM32G0) || defined(STM32L4)
    modify_bits(EXTI->RTSR1,
                1 << pin,
                rising_trigger << pin);
    modify_bits(EXTI->FTSR1,
                1 << pin,
                falling_trigger << pin);
#endif
}

__STATIC_INLINE void exti_enable_interrupt(uint8_t pin)
{
#if defined(STM32F0) || defined(STM32F1)
    set_bits(EXTI->IMR,
             get_bitvalue(pin));
#elif defined(STM32G0) || defined(STM32L4)
    set_bits(EXTI->IMR1,
             get_bitvalue(pin));
#endif
}

__STATIC_INLINE void exti_disable_interrupt(uint8_t pin)
{
#if defined(STM32F0) || defined(STM32F1)
    clear_bits(EXTI->IMR,
               get_bitvalue(pin));
#elif defined(STM32G0) || defined(STM32L4)
    clear_bits(EXTI->IMR1,
               get_bitvalue(pin));
#endif
}

__STATIC_INLINE void exti_clear_pending_interrupt(uint8_t pin)
{
#if defined(STM32F0) || defined(STM32F1)
    set_bits(EXTI->PR,
             get_bitvalue(pin));
#elif defined(STM32G0)
    set_bits(EXTI->RPR1,
             get_bitvalue(pin));
    set_bits(EXTI->FPR1,
             get_bitvalue(pin));
#elif defined(STM32L4)
    set_bits(EXTI->PR1,
             get_bitvalue(pin));
#endif
}

// ADC

#if defined(STM32F0)
#define ADC_TEMP_CHANNEL 16
#define ADC_VREF_CHANNEL 17
#define ADC_VBAT_CHANNEL 18
#elif defined(STM32F1)
#define ADC_TEMP_CHANNEL 16
#define ADC_VREF_CHANNEL 17
#elif defined(STM32G0)
#define ADC_TEMP_CHANNEL 12
#define ADC_VREF_CHANNEL 13
#define ADC_VBAT_CHANNEL 14
#elif defined(STM32L4)
#define ADC_VREF_CHANNEL 0
#define ADC_TEMP_CHANNEL 17
#define ADC_VBAT_CHANNEL 18
#define ADC_VREF_CHANNEL 0
#endif

#if defined(STM32F0) && defined(GD32)

typedef struct
{
    __IO uint32_t STAT;   // ADC status register,                           Address offset: 0x00
    __IO uint32_t CTL0;   // ADC control register 0,                        Address offset: 0x04
    __IO uint32_t CTL1;   // ADC control register 1,                        Address offset: 0x08
    __IO uint32_t SAMPT0; // ADC sampling time register 1,                  Address offset: 0x0C
    __IO uint32_t SAMPT1; // ADC sampling time register 2,                  Address offset: 0x10
    __IO uint32_t IOFF0;  // ADC inserted channel data offset register 0,   Address offset: 0x14
    __IO uint32_t IOFF1;  // ADC inserted channel data offset register 1,   Address offset: 0x18
    __IO uint32_t IOFF2;  // ADC inserted channel data offset register 2,   Address offset: 0x1c
    __IO uint32_t IOFF3;  // ADC inserted channel data offset register 3,   Address offset: 0x20
    __IO uint32_t WDHT;   // ADC watchdog high threshold register,          Address offset: 0x24
    __IO uint32_t WDLT;   // ADC watchdog low threshold register,           Address offset: 0x28
    __IO uint32_t RSQ0;   // ADC routine sequence register 0,               Address offset: 0x2c
    __IO uint32_t RSQ1;   // ADC routine sequence register 1,               Address offset: 0x30
    __IO uint32_t RSQ2;   // ADC routine sequence register 2,               Address offset: 0x34
    __IO uint32_t ISQ;    // ADC inverted sequence register,                Address offset: 0x38
    __IO uint32_t IDATA0; // ADC inverted data register 0,                  Address offset: 0x3c
    __IO uint32_t IDATA1; // ADC inverted data register 1,                  Address offset: 0x40
    __IO uint32_t IDATA2; // ADC inverted data register 2,                  Address offset: 0x44
    __IO uint32_t IDATA3; // ADC inverted data register 3,                  Address offset: 0x48
    __IO uint32_t RDATA;  // ADC routine data register,                     Address offset: 0x4c
} ADC_GD32_TypeDef;

#define ADC_CTL1_ADCON_Pos (0U)
#define ADC_CTL1_ADCON_Msk (0x1UL << ADC_CTL1_ADCON_Pos) // 0x00000001
#define ADC_CTL1_ADCON ADC_CTL1_ADCON_Msk                // ADC ON

#define ADC_CTL1_CLB_Pos (2U)
#define ADC_CTL1_CLB_Msk (0x1UL << ADC_CTL1_CLB_Pos) // 0x00000004
#define ADC_CTL1_CLB ADC_CTL1_CLB_Msk                // ADC calibration

#define ADC_CTL1_RSTCLB_Pos (3U)
#define ADC_CTL1_RSTCLB_Msk (0x1UL << ADC_CTL1_RSTCLB_Pos) // 0x00000008
#define ADC_CTL1_RSTCLB ADC_CTL1_RSTCLB_Msk                // Reset calibration

#define ADC_CTL1_ETSRC_Pos (17U)
#define ADC_CTL1_ETSRC_Msk (0x7UL << ADC_CTL1_ETSRC_Pos)    // 0x000E0000
#define ADC_CTL1_ETSRC ADC_CTL1_ETSRC_Msk                   // External trigger select for routine sequence
#define ADC_CTL1_ETSRC_0 (0x1UL << ADC_CTL1_ETSRC_Pos)      // 0x00020000
#define ADC_CTL1_ETSRC_1 (0x2UL << ADC_CTL1_ETSRC_Pos)      // 0x00040000
#define ADC_CTL1_ETSRC_2 (0x4UL << ADC_CTL1_ETSRC_Pos)      // 0x00080000
#define ADC_CTL1_ETSRC_TIM0CH0 (0x0 << ADC_CTL1_ETSRC_Pos)  // TIMER0 CH0
#define ADC_CTL1_ETSRC_TIM0CH1 (0x1 << ADC_CTL1_ETSRC_Pos)  // TIMER0 CH1
#define ADC_CTL1_ETSRC_TIM0CH2 (0x2 << ADC_CTL1_ETSRC_Pos)  // TIMER0 CH2
#define ADC_CTL1_ETSRC_TIM1CH1 (0x3 << ADC_CTL1_ETSRC_Pos)  // TIMER1 CH1
#define ADC_CTL1_ETSRC_TIM2TRGO (0x4 << ADC_CTL1_ETSRC_Pos) // TIMER2 TRGO
#define ADC_CTL1_ETSRC_TIM3CH0 (0x5 << ADC_CTL1_ETSRC_Pos)  // TIMER3 CH3
#define ADC_CTL1_ETSRC_EXTI11 (0x6 << ADC_CTL1_ETSRC_Pos)   // EXTI line 11
#define ADC_CTL1_ETSRC_SWRCST (0x7 << ADC_CTL1_ETSRC_Pos)   // SWRCST

#define ADC_CTL1_ETERC_Pos (20U)
#define ADC_CTL1_ETERC_Msk (0x1UL << ADC_CTL1_ETERC_Pos) // 0x00100000
#define ADC_CTL1_ETERC ADC_CTL1_ETERC_Msk                // External trigger enable for routine sequence

#define ADC_CTL1_SWRCST_Pos (22U)
#define ADC_CTL1_SWRCST_Msk (0x1UL << ADC_CTL1_SWRCST_Pos) // 0x00400000
#define ADC_CTL1_SWRCST ADC_CTL1_SWRCST_Msk                // Software start conversion of routine sequence

#define ADC_CTL1_TSVREN_Pos (23U)
#define ADC_CTL1_TSVREN_Msk (0x1UL << ADC_CTL1_TSVREN_Pos) // 0x00800000
#define ADC_CTL1_TSVREN ADC_CTL1_TSVREN_Msk                // Channel 16 and 17 enable of ADC

#define ADC_CTL1_VBATEN_Pos (24U)
#define ADC_CTL1_VBATEN_Msk (0x1UL << ADC_CTL1_VBATEN_Pos) // 0x01000000
#define ADC_CTL1_VBATEN ADC_CTL1_VBATEN_Msk                // Enable/disable the VBAT channel

#elif defined(STM32F1)

#define ADC_CR2_EXTSEL_TIM0CH0 (0x0 << ADC_CR2_EXTSEL_Pos)  // TIM1 CC1 event
#define ADC_CR2_EXTSEL_TIM0CH1 (0x1 << ADC_CR2_EXTSEL_Pos)  // TIM1 CC2 event
#define ADC_CR2_EXTSEL_TIM0CH2 (0x2 << ADC_CR2_EXTSEL_Pos)  // TIM1 CC3 event
#define ADC_CR2_EXTSEL_TIM1CH1 (0x3 << ADC_CR2_EXTSEL_Pos)  // TIM2 CC2 event
#define ADC_CR2_EXTSEL_TIM2TRGO (0x4 << ADC_CR2_EXTSEL_Pos) // TIM3 TRGO event
#define ADC_CR2_EXTSEL_TIM3CH0 (0x5 << ADC_CR2_EXTSEL_Pos)  // TIM4 CC4 event
#define ADC_CR2_EXTSEL_EXTI11 (0x6 << ADC_CR2_EXTSEL_Pos)   // EXTI line 11
#define ADC_CR2_EXTSEL_SWSTART (0x7 << ADC_CR2_EXTSEL_Pos)  // SWSTART

#endif

#if defined(STM32L4)

__STATIC_INLINE void adc_enable_deep_power_down(ADC_TypeDef *base)
{
    set_bits(base->CR,
             ADC_CR_DEEPPWD);
}

__STATIC_INLINE void adc_disable_deep_power_down(ADC_TypeDef *base)
{
    clear_bits(base->CR,
               ADC_CR_DEEPPWD);
}

#endif

#if defined(STM32G0) || defined(STM32L4)

__STATIC_INLINE void adc_enable_vreg(ADC_TypeDef *base)
{
    set_bits(base->CR,
             ADC_CR_ADVREGEN);
}

__STATIC_INLINE void adc_disable_vreg(ADC_TypeDef *base)
{
    clear_bits(base->CR,
               ADC_CR_ADVREGEN);
}

#endif

__STATIC_INLINE void adc_enable(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    if (!get_bits(((ADC_GD32_TypeDef *)base)->CTL1,
                  ADC_CTL1_ADCON))
        set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
                 ADC_CTL1_ADCON);
#elif defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    set_bits(base->CR,
             ADC_CR_ADEN);
#elif defined(STM32F1)
    if (!get_bits(base->CR2,
                  ADC_CR2_ADON))
        set_bits(base->CR2,
                 ADC_CR2_ADON);
#endif
}

__STATIC_INLINE void adc_disable(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    clear_bits(((ADC_GD32_TypeDef *)base)->CTL1,
               ADC_CTL1_ADCON);
#elif defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    set_bits(base->CR,
             ADC_CR_ADDIS);
#elif defined(STM32F1)
    clear_bits(base->CR2,
               ADC_CR2_ADON);
#endif
}

__STATIC_INLINE void adc_reset_calibration(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
             ADC_CTL1_RSTCLB);
#elif defined(STM32F1)
    set_bits(base->CR2,
             ADC_CR2_RSTCAL);
#endif
}

__STATIC_INLINE void adc_start_calibration(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
             ADC_CTL1_CLB);
#elif defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    set_bits(base->CR,
             ADC_CR_ADCAL);
#elif defined(STM32F1)
    set_bits(base->CR2,
             ADC_CR2_CAL);
#endif
}

__STATIC_INLINE void adc_enable_vref_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
             ADC_CCR_VREFEN);
#elif defined(STM32L4)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
             ADC_CCR_VREFEN);
#endif
}

__STATIC_INLINE void adc_disable_vref_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
               ADC_CCR_VREFEN);
#elif defined(STM32L4)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
               ADC_CCR_VREFEN);
#endif
}

__STATIC_INLINE void adc_enable_vbat_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
             ADC_CTL1_VBATEN);
#elif defined(STM32F0) || defined(STM32G0)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
             ADC_CCR_VBATEN);
#elif defined(STM32L4)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
             ADC_CCR_VBATEN);
#endif
}

__STATIC_INLINE void adc_disable_vbat_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    clear_bits(((ADC_GD32_TypeDef *)base)->CTL1,
               ADC_CTL1_VBATEN);
#elif defined(STM32F0) || defined(STM32G0)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
               ADC_CCR_VBATEN);
#elif defined(STM32L4)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
               ADC_CCR_VBATEN);
#endif
}

__STATIC_INLINE void adc_enable_temperature_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
             ADC_CCR_TSEN);
#elif defined(STM32L4)
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
             ADC_CCR_TSEN);
#endif
}

__STATIC_INLINE void adc_disable_temperature_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x308))->CCR,
               ADC_CCR_TSEN);
#elif defined(STM32L4)
    clear_bits(((ADC_Common_TypeDef *)((uint8_t *)base + 0x300))->CCR,
               ADC_CCR_TSEN);
#endif
}

__STATIC_INLINE void adc_enable_temperature_vref_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
             ADC_CTL1_TSVREN);
#elif defined(STM32F1)
    set_bits(base->CR2,
             ADC_CR2_TSVREFE);
#endif
}

__STATIC_INLINE void adc_disable_temperature_vref_channel(ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    clear_bits(((ADC_GD32_TypeDef *)base)->CTL1,
               ADC_CTL1_TSVREN);
#elif defined(STM32F1)
    clear_bits(base->CR2,
               ADC_CR2_TSVREFE);
#endif
}

__STATIC_INLINE void adc_start_conversion_oneshot(ADC_TypeDef *base,
                                                  uint8_t channel,
                                                  uint8_t sample_time)
{
#if defined(STM32F0) && defined(GD32)
    ((ADC_GD32_TypeDef *)base)->RSQ0 = 0;
    ((ADC_GD32_TypeDef *)base)->RSQ2 = channel;

    modify_bits(((ADC_GD32_TypeDef *)base)->CTL1,
                ADC_CTL1_ETERC_Msk | ADC_CTL1_ETSRC_Msk,
                ADC_CTL1_ETERC | ADC_CTL1_ETSRC_SWRCST);

    if (channel >= 10)
        ((ADC_GD32_TypeDef *)base)->SAMPT0 = sample_time << (3 * (channel - 10));
    else
        ((ADC_GD32_TypeDef *)base)->SAMPT1 = sample_time << (3 * channel);

    set_bits(((ADC_GD32_TypeDef *)base)->CTL1,
             ADC_CTL1_SWRCST);
#elif defined(STM32F0) || defined(STM32G0)
    base->CHSELR = get_bitvalue(channel);
    base->SMPR = sample_time;

    set_bits(base->CR,
             ADC_CR_ADSTART);
#elif defined(STM32F1)
    base->SQR1 = 0;
    base->SQR3 = channel;

    modify_bits(base->CR2,
                ADC_CR2_EXTTRIG_Msk | ADC_CR2_EXTSEL_Msk,
                ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL_SWSTART);

    if (channel >= 10)
        base->SMPR1 = sample_time << (3 * (channel - 10));
    else
        base->SMPR2 = sample_time << (3 * channel);

    set_bits(base->CR2,
             ADC_CR2_SWSTART);
#elif defined(STM32L4)
    base->SQR1 = (channel << ADC_SQR1_SQ1_Pos);

    if (channel < 10)
        base->SMPR1 = sample_time << (3 * channel);
    else
        base->SMPR2 = sample_time << (3 * (channel - 10));

    set_bits(base->CFGR,
             ADC_CFGR_OVRMOD);

    set_bits(base->CR,
             ADC_CR_ADSTART);
#endif
}

__STATIC_INLINE uint32_t adc_get_conversion_oneshot(const ADC_TypeDef *base)
{
#if defined(STM32F0) && defined(GD32)
    return ((ADC_GD32_TypeDef *)base)->RDATA;
#else
    return base->DR;
#endif
}

// TIM

#define TIM_CH1 0
#define TIM_CH2 1
#define TIM_CH3 2
#define TIM_CH4 3

#define TIM_CR2_MMS_RESET (0b000 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_ENABLE (0b001 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_UPDATE (0b010 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_COMPARE_PULSE (0b011 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_COMPARE_OC1REF (0b100 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_COMPARE_OC2REF (0b101 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_COMPARE_OC3REF (0b110 << TIM_CR2_MMS_Pos)
#define TIM_CR2_MMS_COMPARE_OC4REF (0b111 << TIM_CR2_MMS_Pos)

#define TIM_SMCR_TS_ITR0 (0b000 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_ITR1 (0b001 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_ITR2 (0b010 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_ITR3 (0b011 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_TI1F_ED (0b100 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_TI1FP1 (0b101 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_TI2FP2 (0b110 << TIM_SMCR_TS_Pos)
#define TIM_SMCR_TS_ETRF (0b111 << TIM_SMCR_TS_Pos)

#define TIM_SMCR_SMS_OFF (0b000 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_EM1 (0b001 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_EM2 (0b010 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_EM3 (0b011 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_RM (0b100 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_GM (0b101 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_TM (0b110 << TIM_SMCR_SMS_Pos)
#define TIM_SMCR_SMS_ECM1 (0b111 << TIM_SMCR_SMS_Pos)

__STATIC_INLINE void tim_enable(TIM_TypeDef *base)
{
    set_bits(base->CR1,
             TIM_CR1_CEN);
}

__STATIC_INLINE void tim_disable(TIM_TypeDef *base)
{
    clear_bits(base->CR1,
               TIM_CR1_CEN);
}

__STATIC_INLINE void tim_generate_update(TIM_TypeDef *base)
{
    set_bits(base->EGR,
             TIM_EGR_UG);
}

__STATIC_INLINE void tim_set_period(TIM_TypeDef *base,
                                    uint32_t value)
{
    base->ARR = value - 1;
}

__STATIC_INLINE void tim_set_prescaler_factor(TIM_TypeDef *base,
                                              uint32_t value)

{
    base->PSC = value - 1;
}

__STATIC_INLINE void tim_set_ontime(TIM_TypeDef *base,
                                    uint32_t channel,
                                    uint32_t ontime)
{
    switch (channel)
    {
    case TIM_CH1:
        base->CCR1 = ontime;

        break;

    case TIM_CH2:
        base->CCR2 = ontime;

        break;

    case TIM_CH3:
        base->CCR3 = ontime;

        break;

    case TIM_CH4:
        base->CCR4 = ontime;

        break;
    }
}

__STATIC_INLINE void tim_setup_pwm(TIM_TypeDef *base,
                                   uint32_t channel)
{
    rcc_enable_tim(base);

    switch (channel)
    {
    case TIM_CH1:
        modify_bits(base->CCMR1,
                    TIM_CCMR1_OC1M_Msk,
                    TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        set_bits(base->CCER,
                 TIM_CCER_CC1E);

        break;

    case TIM_CH2:
        modify_bits(base->CCMR1,
                    TIM_CCMR1_OC2M_Msk,
                    TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        set_bits(base->CCER,
                 TIM_CCER_CC2E);

        break;

    case TIM_CH3:
        modify_bits(base->CCMR2,
                    TIM_CCMR2_OC3M_Msk,
                    TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        set_bits(base->CCER,
                 TIM_CCER_CC3E);

        break;

    case TIM_CH4:
        modify_bits(base->CCMR2,
                    TIM_CCMR2_OC4M_Msk,
                    TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        set_bits(base->CCER,
                 TIM_CCER_CC4E);

        break;
    }

    if (base == TIM1)
        set_bits(base->BDTR, TIM_BDTR_MOE);
}

__STATIC_INLINE void tim_setup_linked(TIM_TypeDef *base_master,
                                      TIM_TypeDef *base_slave,
                                      uint8_t trigger_connection)
{
    rcc_enable_tim(base_master);
    set_bits(base_master->CR2,
             TIM_CR2_MMS_UPDATE);
    base_master->ARR = 0xffff;

    rcc_enable_tim(base_slave);
    set_bits(base_slave->SMCR,
             (trigger_connection << TIM_SMCR_TS_Pos) | TIM_SMCR_SMS_ECM1);
    base_slave->ARR = 0xffff;

    tim_generate_update(base_master);
}

// IWDG

__STATIC_INLINE void iwdg_unlock(void)
{
    IWDG->KR = 0x5555;
}

__STATIC_INLINE void iwdg_start(void)
{
    IWDG->KR = 0xcccc;
}

__STATIC_INLINE void iwdg_reload(void)
{
    IWDG->KR = 0xaaaa;
}

// RTC

#if defined(STM32G0) || defined(STM32L4)
#define RCC_BDCR_RTCSEL_NOCLOCK (0x00000000U) /*!< No clock */
#define RCC_BDCR_RTCSEL_LSE (0x00000100U)     /*!< LSE oscillator clock used as RTC clock */
#define RCC_BDCR_RTCSEL_LSI (0x00000200U)     /*!< LSI oscillator clock used as RTC clock */
#define RCC_BDCR_RTCSEL_HSE (0x00000300U)     /*!< HSE oscillator clock divided by 128 used as RTC clock */
#endif

__STATIC_INLINE void rtc_disable_backup_domain_write_protection(void)
{
#if defined(STM32F0) || defined(STM32F1)
    set_bits(PWR->CR,
             PWR_CR_DBP);
#elif defined(STM32G0) || defined(STM32L4)
    set_bits(PWR->CR1,
             PWR_CR1_DBP);
#endif
}

__STATIC_INLINE void rtc_enable_lse(void)
{
    set_bits(RCC->BDCR,
             RCC_BDCR_LSEON);
}

__STATIC_INLINE bool rtc_is_lse_ready(void)
{
    return get_bits(RCC->BDCR,
                    RCC_BDCR_LSERDY);
}

__STATIC_INLINE bool rtc_is_enabled(void)
{
    return get_bits(RCC->BDCR,
                    RCC_BDCR_RTCEN);
}

__STATIC_INLINE void rtc_enable(void)
{
    modify_bits(RCC->BDCR,
                RCC_BDCR_RTCSEL_Msk,
                RCC_BDCR_RTCSEL_LSE);

    set_bits(RCC->BDCR,
             RCC_BDCR_RTCEN);
}

__STATIC_INLINE void rtc_enter_configuration_mode(void)
{
#if defined(STM32F0)
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;

    set_bits(RTC->ISR,
             RTC_ISR_INIT);
    wait_until_bits_set(RTC->ISR,
                        RTC_ISR_INITF);
#elif defined(STM32F1)
    wait_until_bits_set(RTC->CRL,
                        RTC_CRL_RTOFF);
    set_bits(RTC->CRL,
             RTC_CRL_CNF);
#elif defined(STM32G0)
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;

    set_bits(RTC->ICSR,
             RTC_ICSR_INIT);
    wait_until_bits_set(RTC->ICSR,
                        RTC_ICSR_INITF);
#elif defined(STM32L4)
    RTC->WPR = 0xca;
    RTC->WPR = 0x53;

    set_bits(RTC->ISR,
             RTC_ISR_INIT);
    wait_until_bits_set(RTC->ISR,
                        RTC_ISR_INITF);
#endif
}

__STATIC_INLINE void rtc_leave_configuration_mode(void)
{
#if defined(STM32F0)
    clear_bits(RTC->ISR,
               RTC_ISR_INIT);

    clear_bits(RTC->ISR,
               RTC_ISR_RSF);
    wait_until_bits_set(RTC->ISR,
                        RTC_ISR_RSF);

    RTC->WPR = 0xfe;
    RTC->WPR = 0x64;
#elif defined(STM32F1)
    clear_bits(RTC->CRL,
               RTC_CRL_CNF);
    wait_until_bits_set(RTC->CRL,
                        RTC_CRL_RTOFF);
#elif defined(STM32G0)
    clear_bits(RTC->ICSR,
               RTC_ICSR_INIT);

    clear_bits(RTC->ICSR,
               RTC_ICSR_RSF);
    wait_until_bits_set(RTC->ICSR,
                        RTC_ICSR_RSF);

    RTC->WPR = 0xfe;
    RTC->WPR = 0x64;
#elif defined(STM32L4)
    clear_bits(RTC->ISR,
               RTC_ISR_INIT);

    clear_bits(RTC->ISR,
               RTC_ISR_RSF);
    wait_until_bits_set(RTC->ISR,
                        RTC_ISR_RSF);

    RTC->WPR = 0xfe;
    RTC->WPR = 0x64;
#endif
}

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

__STATIC_INLINE void rtc_get_date_time(uint32_t *dr,
                                       uint32_t *tr)
{
    *tr = RTC->TR;
    *dr = RTC->DR;
}

__STATIC_INLINE void rtc_set_date_time(uint32_t dr,
                                       uint32_t tr)
{
    RTC->TR = tr;
    RTC->DR = dr;
}

#elif defined(STM32F1)

__STATIC_INLINE void rtc_set_prescaler_factor(uint32_t value)
{
    value--;

    RTC->PRLH = (value >> 16) & 0xffff;
    RTC->PRLL = value & 0xffff;
}

__STATIC_INLINE uint32_t rtc_get_count(void)
{
    uint32_t countHigh1 = RTC->CNTH;
    uint32_t count = RTC->CNTL;
    uint32_t countHigh2 = RTC->CNTH;

    if (countHigh1 == countHigh2)
        return count | (countHigh1 << 16);
    else
    {
        if (count & 0x8000)
            return count | (countHigh1 << 16);
        else
            return count | (countHigh2 << 16);
    }
}

__STATIC_INLINE void rtc_set_count(uint32_t value)
{
    RTC->CNTL = 0;
    RTC->CNTH = (value >> 16) & 0xffff;
    RTC->CNTL = value & 0xffff;
}

#endif

// USART

#define USART_CR1_M_8BITS 0
#define USART_CR1_M_9BITS USART_CR1_M

#define USART_CR1_PCE_NONE 0
#define USART_CR1_PCE_EVEN USART_CR1_PCE

#define USART_CR2_STOP_0_5BIT USART_CR2_STOP_0
#define USART_CR2_STOP_1BIT 0
#define USART_CR2_STOP_1_5BIT (USART_CR2_STOP_1 | USART_CR2_STOP0)
#define USART_CR2_STOP_2BIT USART_CR2_STOP_1

__STATIC_INLINE void usart_setup_8n1(USART_TypeDef *base,
                                     uint32_t baud_rate)
{
    rcc_enable_usart(base);

    base->BRR = baud_rate;
    base->CR1 = USART_CR1_UE |       // Enable USART
                USART_CR1_M_8BITS |  // 8 bits
                USART_CR1_PCE_NONE | // Parity none
                USART_CR1_TE |       // Enable transmitter
                USART_CR1_RE;        // Enable receiver
    base->CR2 = USART_CR2_STOP_1BIT; // 1 stop bit
    base->CR3 = 0;                   // No flow control
}

__STATIC_INLINE void usart_enable_receive_interrupt(USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    set_bits(base->CR1,
             USART_CR1_RXNEIE);
#elif defined(STM32G0)
    set_bits(base->CR1,
             USART_CR1_RXNEIE_RXFNEIE);
#endif
}

__STATIC_INLINE void usart_disable_receive_interrupt(USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    clear_bits(base->CR1,
               USART_CR1_RXNEIE);
#elif defined(STM32G0)
    clear_bits(base->CR1,
               USART_CR1_RXNEIE_RXFNEIE);
#endif
}

__STATIC_INLINE bool usart_is_receive_interrupt_enabled(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    return get_bits(base->CR1,
                    USART_CR1_RXNEIE);
#elif defined(STM32G0)
    return get_bits(base->CR1,
                    USART_CR1_RXNEIE_RXFNEIE);
#endif
}

__STATIC_INLINE void usart_enable_transmit_interrupt(USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    set_bits(base->CR1,
             USART_CR1_TXEIE);
#elif defined(STM32G0)
    set_bits(base->CR1,
             USART_CR1_TXEIE_TXFNFIE);
#endif
}

__STATIC_INLINE void usart_disable_transmit_interrupt(USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    clear_bits(base->CR1,
               USART_CR1_TXEIE);
#elif defined(STM32G0)
    clear_bits(base->CR1,
               USART_CR1_TXEIE_TXFNFIE);
#endif
}

__STATIC_INLINE bool usart_is_transmit_interrupt_enabled(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32F1) || defined(STM32L4)
    return get_bits(base->CR1,
                    USART_CR1_TXEIE);
#elif defined(STM32G0)
    return get_bits(base->CR1,
                    USART_CR1_TXEIE_TXFNFIE);
#endif
}

__STATIC_INLINE bool usart_is_receive_ready(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32L4)
    return get_bits(base->ISR,
                    USART_ISR_RXNE);
#elif defined(STM32F1)
    return get_bits(base->SR,
                    USART_SR_RXNE);
#elif defined(STM32G0)
    return get_bits(base->ISR,
                    USART_ISR_RXNE_RXFNE);
#endif
}

__STATIC_INLINE bool usart_is_send_ready(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32L4)
    return get_bits(base->ISR,
                    USART_ISR_TXE);
#elif defined(STM32F1)
    return get_bits(base->SR,
                    USART_SR_TXE);
#elif defined(STM32G0)
    return get_bits(base->ISR,
                    USART_ISR_TXE_TXFNF);
#endif
}

__STATIC_INLINE bool usart_is_overrun(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    return get_bits(base->ISR,
                    USART_ISR_ORE);
#elif defined(STM32F1)
    return get_bits(base->SR,
                    USART_SR_ORE);
#endif
}

__STATIC_INLINE void usart_clear_overrun(USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    set_bits(base->ICR,
             USART_ICR_ORECF);
#elif defined(STM32F1)
    base->DR;
#endif
}

__STATIC_INLINE uint16_t usart_receive(const USART_TypeDef *base)
{
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    return base->RDR;
#elif defined(STM32F1)
    return base->DR;
#endif
}

__STATIC_INLINE uint16_t usart_receive_blocking(const USART_TypeDef *base)
{
    while (!usart_is_receive_ready(base))
        ;
    return usart_receive(base);
}

__STATIC_INLINE void usart_send(USART_TypeDef *base,
                                uint16_t data)
{
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    base->TDR = data;
#elif defined(STM32F1)
    base->DR = data;
#endif
}

__STATIC_INLINE void usart_send_blocking(USART_TypeDef *base,
                                         uint16_t data)
{
    while (!usart_is_send_ready(base))
        ;
    usart_send(base, data);
}

// SPI

__STATIC_INLINE void spi_send(SPI_TypeDef *base,
                              uint16_t data)
{
    wait_until_bits_set(base->SR,
                        SPI_SR_TXE);

    base->DR = data;
}

// ROM

#if defined(STM32F0)
#define TS_CAL1 (*((__I uint16_t *)0x1ffff7b8))
#define TS_CAL2 (*((__I uint16_t *)0x1ffff7c2))
#define VREFINT_CAL_VALUE (*((__I uint16_t *)0x1ffff7ba))
#define VREFINT_CAL_VOLTAGE 3.3F
#elif defined(STM32G0)
#define TS_CAL1 (*((__I uint16_t *)0x1fff75a8))
#define VREFINT_CAL_VALUE (*((__I uint16_t *)0x1fff75aa))
#define VREFINT_CAL_VOLTAGE 3.0F
#elif defined(STM32L4)
#define TS_CAL1 (*((__I uint16_t *)0x1fff75a8))
#define TS_CAL2 (*((__I uint16_t *)0x1fff75ca))
#define VREFINT_CAL_VALUE (*((__I uint16_t *)0x1fff75aa))
#define VREFINT_CAL_VOLTAGE 3.0F
#endif

#define UID0 (((__I uint32_t *)UID_BASE)[0])
#define UID1 (((__I uint32_t *)UID_BASE)[1])
#define UID2 (((__I uint32_t *)UID_BASE)[2])

#endif
