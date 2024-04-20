/*
 * CMSIS compatibility header
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 * Based on: https://github.com/dmitrystu/stm32h
 */

#ifndef _STM32_H_
#define _STM32_H_

// Get bits
#define get_bits(reg, bits) ((reg) & (bits))

// Modify bitfield
#define _BMD(reg, msk, val) (reg) = (((reg) & ~(msk)) | (val))
#define modify_bits(reg, msk, val) _BMD(reg, msk, val)

// Set bitfield
#define _BST(reg, bits) (reg) = ((reg) | (bits))
#define set_bits(reg, bits) _BST(reg, bits)

// Clear bitfield
#define _BCL(reg, bits) (reg) = ((reg) & ~(bits))
#define clear_bits(reg, bits) _BCL(reg, bits)

// Wait until bitfield set
#define _WBS(reg, bits) while (get_bits(reg, bits) == 0)
#define wait_until_bits_set(reg, bits) _WBS(reg, bits)

// Wait until bitfield clear
#define _WBC(reg, bits) while (get_bits(reg, bits) != 0)
#define wait_until_bits_clear(reg, bits) _WBC(reg, bits);

// Wait until bitfield achieves a value
#define _WVL(reg, bits, val) while (get_bits(reg, bits) != (val))
#define wait_until_bits_value(reg, bits, val) _WVL(reg, bits, val)

// Get bit value
#define _BV(pos) (1 << (pos))
#define get_bitvalue(pos) (1 << (pos))

#if defined(STM32F0)
#include "stm32f0xx.h"
#elif defined(STM32F1)
#include "stm32f1xx.h"
#elif defined(STM32F2)
#include "stm32f2xx.h"
#elif defined(STM32F3)
#include "stm32f3xx.h"
#elif defined(STM32F4)
#include "stm32f4xx.h"
#elif defined(STM32F7)
#include "stm32f7xx.h"
#elif defined(STM32H7)
#include "stm32h7xx.h"
#elif defined(STM32L0)
#include "stm32l0xx.h"
#elif defined(STM32L1)
#include "stm32l1xx.h"
#elif defined(STM32L4)
#include "stm32l4xx.h"
#elif defined(STM32L5)
#include "stm32l5xx.h"
#elif defined(STM32G0)
#include "stm32g0xx.h"
#elif defined(STM32G4)
#include "stm32g4xx.h"
#elif defined(STM32WB)
#include "stm32wbxx.h"
#else
#error "STM32 family not defined"
#endif

#endif
