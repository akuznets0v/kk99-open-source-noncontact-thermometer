#include <stdint.h>

/**
 * Initialize 4-digits micro numeric indicator.
 *
 * Part number: HP QDSP-6064
 **/
extern void
SSNIndicatorInit(void);

/**
 * Display given number.
 *
 * Minimum and maximum values for integer numbers:
 * -999 - 9999 (dotPosition = 0)
 *
 * Minimum and maximum values for floating-point numbers:
 * -99.9 - 999.9 (dotPosition = 2)
 * -9.99 - 99.99 (dotPosition = 3)
 * -.999 - 9.999 (dotPosition = 4)
 *
 * If given value is smaller/bigger than allowed range
 * will be displayed respectively -OL/OL.
 *
 * @param[in] number        number to display
 * @param[in] dotPosition   position of dot for floating-point number,
 *                          allowed values [2, 4]
 **/
extern void
SSNIndicatorDisplayNumber(int number, uint8_t dotPosition);
