#include <SSNIndicator.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define SEGMENT_A  PORTB0
#define SEGMENT_B  PORTB1
#define SEGMENT_C  PORTD1
#define SEGMENT_D  PORTB3
#define SEGMENT_E  PORTD2
#define SEGMENT_F  PORTB2
#define SEGMENT_G  PORTB4
#define SEGMENT_DP PORTD0

#define DIGIT_1 PORTD3
#define DIGIT_2 PORTD6
#define DIGIT_3 PORTD4
#define DIGIT_4 PORTD5

#define REFRESH_RATE 2000

static void
SSNIndicatorClearSegments(void)
{
    PORTB &= ~(_BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G));
    PORTD &= ~(_BV(SEGMENT_C) | _BV(SEGMENT_E) | _BV(SEGMENT_DP));
}

static void
SSNIndicatorDisplayMinus(void)
{
    PORTB |= _BV(SEGMENT_G);
}

static void
SSNIndicatorDisplayDot(void)
{
    PORTD |= _BV(SEGMENT_DP);
}

static void
SSNIndicatorEnableDigit(uint8_t i, bool enable)
{
    switch (i) {
        case 0:
            if (enable) {
                PORTD &= ~_BV(DIGIT_1);
            } else {
                PORTD |= _BV(DIGIT_1);
            }
            break;
        case 1:
            if (enable) {
                PORTD &= ~_BV(DIGIT_2);
            } else {
                PORTD |= _BV(DIGIT_2);
            }
            break;
        case 2:
            if (enable) {
                PORTD &= ~_BV(DIGIT_3);
            } else {
                PORTD |= _BV(DIGIT_3);
            }
            break;
        case 3:
            if (enable) {
                PORTD &= ~_BV(DIGIT_4);
            } else {
                PORTD |= _BV(DIGIT_4);
            }
            break;
        default:
            break;
    }
}

static void
SSNIndicatorDisplayDigit(uint8_t digit)
{
    switch (digit) {
    case 0:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_F);
        PORTD |= _BV(SEGMENT_C) | _BV(SEGMENT_E);
        break;
    case 1:
        PORTB |= _BV(SEGMENT_B);
        PORTD |= _BV(SEGMENT_C);
        break;
    case 2:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_E);
        break;
    case 3:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C);
        break;
    case 4:
        PORTB |= _BV(SEGMENT_B) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C);
        break;
    case 5:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C);
        break;
    case 6:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C) | _BV(SEGMENT_E);
        break;
    case 7:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B);
        PORTD |= _BV(SEGMENT_C);
        break;
    case 8:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C) | _BV(SEGMENT_E);
        break;
    case 9:
        PORTB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
        PORTD |= _BV(SEGMENT_C);
        break;
    default:
        break;
    }
}

static void
SSNIndicatorDisplayOL(bool negative)
{
    uint8_t digits = negative ? 3 : 2;
    uint8_t i;
    for (i = 3; i >= 4 - digits; i--) {
        SSNIndicatorEnableDigit(i, true);
        switch (i) {
            case 3:
                PORTB |= _BV(SEGMENT_D) | _BV(SEGMENT_F);
                PORTD |= _BV(SEGMENT_E);
                break;
            case 2:
                SSNIndicatorDisplayDigit(0);
                break;
            case 1:
                SSNIndicatorDisplayMinus();
                break;
            default:
                break;
        }
        _delay_us(REFRESH_RATE);
        SSNIndicatorClearSegments();
        SSNIndicatorEnableDigit(i, false);
    }
}

void
SSNIndicatorDisplayNumber(int number, uint8_t dotPosition)
{
    if (number < -999) {
        SSNIndicatorDisplayOL(true);
        return;
    } else if (number > 9999) {
        SSNIndicatorDisplayOL(false);
        return;
    } else if (dotPosition == 1 && dotPosition > 4) {
        return;
    }

    int8_t i;
    int tmp = number;
    bool minus = tmp < 0;
    int8_t numberOfDigits = 4;
    if (minus) {
        tmp *= -1;
    }

    if (tmp <= 9) {
        numberOfDigits = 1;
    } else if (tmp <= 99) {
        numberOfDigits = 2;
    } else if (tmp <= 999) {
        numberOfDigits = 3;
    }

    int8_t minusPos = -1;
    if (minus && numberOfDigits < 4) {
        numberOfDigits = minus ? numberOfDigits + 1 : numberOfDigits;
        minusPos = 4 - numberOfDigits;
    }

    if (numberOfDigits <= dotPosition) {
        if (minus) {
            if (dotPosition < 4) {
                numberOfDigits = dotPosition + 1;
                minusPos = 4 - numberOfDigits;
            } else {
                return;
            }
        } else {
            numberOfDigits = dotPosition;
        }
    }

    for (i = 3; i >= 4 - numberOfDigits; i--) {
        SSNIndicatorEnableDigit(i, true);

        if (minus && (minusPos != -1) && (minusPos == i)) {
            SSNIndicatorDisplayMinus();
            minus = false;
        } else {
            unsigned int digit = tmp % 10;
            SSNIndicatorDisplayDigit(digit);
            tmp /= 10;
        }

        if (((4 - dotPosition) == i) && (4 - dotPosition != minusPos)) {
            SSNIndicatorDisplayDot();
        }

        _delay_us(REFRESH_RATE);
        SSNIndicatorClearSegments();
        SSNIndicatorEnableDigit(i, false);
    }
}

void
SSNIndicatorInit(void)
{
    // setup ports to output
    DDRB |= _BV(SEGMENT_A) | _BV(SEGMENT_B) | _BV(SEGMENT_D) | _BV(SEGMENT_F) | _BV(SEGMENT_G);
    DDRD |= _BV(SEGMENT_C) | _BV(SEGMENT_DP) | _BV(SEGMENT_E) | _BV(DIGIT_1) | _BV(DIGIT_2) | _BV(DIGIT_3) | _BV(DIGIT_4);

    // set digit cathodes to H
    PORTD |= _BV(DIGIT_1) | _BV(DIGIT_2) | _BV(DIGIT_3) | _BV(DIGIT_4);
}
