/*
 * FS2011 Pro
 * Confidence interval calculation
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef CONFIDENCE_H
#define CONFIDENCE_H

#include <stdint.h>

void getConfidenceIntervals(uint32_t sampleNum,
                            uint32_t *lowerConfidenceInterval,
                            uint32_t *upperConfidenceInterval);

#endif
