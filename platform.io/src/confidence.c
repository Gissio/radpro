/*
 * FS2011 Pro
 * Confidence interval calculation
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "confidence.h"

typedef const struct
{
    uint16_t sampleNum;
    uint8_t lowerInterval;
    uint16_t upperInterval;
} ConfidenceInterval;

ConfidenceInterval confidenceIntervals[] = {
    {1, 73, 3850},
    {2, 65, 726},
    {3, 59, 385},
    {4, 55, 268},
    {5, 52, 208},
    {6, 49, 173},
    {7, 47, 149},
    {8, 45, 132},
    {9, 43, 119},
    {10, 42, 109},
    {11, 41, 101},
    {12, 40, 94},
    {13, 38, 88},
    {14, 38, 83},
    {15, 37, 79},
    {16, 36, 75},
    {17, 35, 72},
    {18, 34, 69},
    {19, 34, 67},
    {20, 33, 64},
    {21, 33, 62},
    {22, 32, 60},
    {23, 31, 58},
    {24, 31, 57},
    {25, 30, 55},
    {26, 30, 54},
    {27, 30, 52},
    {28, 29, 51},
    {29, 29, 50},
    {30, 28, 49},
    {31, 28, 48},
    {32, 28, 47},
    {33, 27, 46},
    {34, 27, 45},
    {35, 27, 44},
    {36, 27, 43},
    {37, 26, 43},
    {38, 26, 42},
    {39, 26, 41},
    {40, 25, 40},
    {42, 25, 39},
    {44, 25, 38},
    {45, 24, 38},
    {46, 24, 37},
    {48, 24, 36},
    {50, 23, 35},
    {52, 23, 34},
    {55, 22, 33},
    {58, 22, 32},
    {61, 22, 31},
    {62, 21, 31},
    {64, 21, 30},
    {68, 21, 29},
    {69, 20, 29},
    {72, 20, 28},
    {76, 20, 27},
    {78, 19, 27},
    {81, 19, 26},
    {87, 19, 25},
    {88, 18, 25},
    {93, 18, 24},
    {100, 18, 23},
    {101, 17, 23},
    {108, 17, 22},
    {116, 16, 22},
    {117, 16, 21},
    {127, 16, 20},
    {134, 15, 20},
    {139, 15, 19},
    {153, 15, 18},
    {157, 14, 18},
    {169, 14, 17},
    {185, 13, 17},
    {188, 13, 16},
    {211, 13, 15},
    {221, 12, 15},
    {239, 12, 14},
    {267, 11, 14},
    {274, 11, 13},
    {317, 11, 12},
    {328, 10, 12},
    {372, 10, 11},
    {412, 9, 11},
    {444, 9, 10},
    {530, 8, 10},
    {541, 8, 9},
    {675, 8, 8},
    {703, 7, 8},
    {869, 7, 7},
    {973, 6, 7},
    {1166, 6, 6},
    {1423, 5, 6},
    {1654, 5, 5},
    {2258, 4, 5},
    {2548, 4, 4},
    {4077, 3, 4},
    {4463, 3, 3},
    {9316, 2, 3},
    {9895, 2, 2},
    {37838, 1, 2},
    {38995, 1, 1},
};

#define CONFIDENCE_INTERVALS_SIZE (sizeof(confidenceIntervals) / sizeof(ConfidenceInterval))

void getConfidenceIntervals(uint32_t sampleNum,
                            uint32_t *lowerConfidenceInterval,
                            uint32_t *upperConfidenceInterval)
{
    int left = 0;
    int right = CONFIDENCE_INTERVALS_SIZE - 1;
    int index = -1;

    while (left <= right)
    {
        int mid = (left + right) / 2;
        uint32_t confidenceSampleNum = confidenceIntervals[mid].sampleNum;

        if (confidenceSampleNum < sampleNum)
            left = mid + 1;
        else if (confidenceSampleNum > sampleNum)
            right = mid - 1;
        else
        {
            index = mid;
            break;
        }
    }

    if (index == -1)
        index = right;

    *lowerConfidenceInterval = confidenceIntervals[index].lowerInterval;
    *upperConfidenceInterval = confidenceIntervals[index].upperInterval;
}
