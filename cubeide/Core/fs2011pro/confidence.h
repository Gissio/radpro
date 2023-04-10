/*
 * FS2011 Pro
 * Confidence interval calculation
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef CONFIDENCE_H
#define CONFIDENCE_H

#include <stdint.h>

void getConfidenceIntervals(uint32_t sampleNum,
                            int *lowerConfidenceInterval,
                            int *upperConfidenceInterval);

#endif
