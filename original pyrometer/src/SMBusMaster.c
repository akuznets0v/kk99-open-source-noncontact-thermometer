#include <SMBusMaster.h>
#include <avr/io.h>
#include <util/delay.h>

// USI pinout configuration
#define DDR_USI      DDRB
#define PIN_USI      PINB
#define PORT_USI     PORTB
#define PIN_USI_SDA  PINB5
#define PIN_USI_SCL  PINB7
#define PORT_USI_SDA PORTB5
#define PORT_USI_SCL PORTB7

#define NACK_BIT 0

#define T_LOW  4.7 // low period of the SCK clock
#define T_HIGH 4.0 // high period of the SCK clock
#define T_BUF  4.7 // bus free time between a stop and start condition
#define T_HD   4.0 // hold time (repeated) start condition
#define T_SU   4.0 // set-up time for stop condition

typedef enum {
    SMBusMasterTransferSize_1bit,
    SMBusMasterTransferSize_8bit
} SMBusMasterTransferSize;

static void
SMBusMasterTransferData(SMBusMasterTransferSize size)
{
    if (size == SMBusMasterTransferSize_1bit) {
        USISR |= _BV(USICNT3) | _BV(USICNT2) | _BV(USICNT1);
    } else {
        USISR &= ~(_BV(USICNT3) | _BV(USICNT2) | _BV(USICNT1) | _BV(USICNT0));
    }

    do {
        USICR |= _BV(USITC);
        while (!(PIN_USI & _BV(PIN_USI_SCL)));
        _delay_us(T_LOW);

        USICR |= _BV(USITC);
        _delay_us(T_HIGH);
    } while (!(USISR & _BV(USIOIF)));
}

static void
SMBusMasterWriteData(uint8_t data, SMBusMasterTransferSize size)
{
    USIDR = data;
    SMBusMasterTransferData(size);
    USIDR = 0xFF;
}

static uint8_t
SMBusMasterReadData(SMBusMasterTransferSize size)
{
    uint8_t data = 0;

    DDR_USI &= ~_BV(PORT_USI_SDA);
    SMBusMasterTransferData(size);

    data = USIDR;
    USIDR = 0xFF;
    DDR_USI |= _BV(PORT_USI_SDA);

    return data;
}

void
SMBusMasterInit(void)
{
    // setup SDA and SCL as outputs
    DDR_USI |= _BV(PORT_USI_SDA) | _BV(PORT_USI_SCL);

    // set SDA and SCL to H
    PORT_USI |= _BV(PORT_USI_SDA) | _BV(PORT_USI_SCL);

    USIDR = 0xFF;
    // setup two-wire mode and software clock strobe
    USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK);

    // clear flags
    USISR = _BV(USISIF) | _BV(USIOIF) | _BV(USIPF) | _BV(USIDC);
}

int8_t
SMBusMasterStart(uint8_t address)
{
    PORT_USI |= _BV(PORT_USI_SCL);
    while (!(PIN_USI & _BV(PIN_USI_SCL)));

    _delay_us(T_BUF);

    PORT_USI &= ~_BV(PORT_USI_SDA);

    _delay_us(T_HD);

    PORT_USI &= ~_BV(PORT_USI_SCL);
    PORT_USI |= _BV(PORT_USI_SDA);

    //send address and r/w mode
    SMBusMasterWriteData(address, SMBusMasterTransferSize_8bit);

    //read ACK
    if (SMBusMasterReadData(SMBusMasterTransferSize_1bit) & _BV(NACK_BIT)) {
        // no ack from slave
        return -1;
    }

    return 0;
}

int8_t
SMBusMasterRepeatedStart(uint8_t address)
{
    return SMBusMasterStart(address);
}

int8_t
SMBusMasterWrite(uint8_t data)
{
    //send address and r/w mode
    SMBusMasterWriteData(data, SMBusMasterTransferSize_8bit);

    //read ACK
    if (SMBusMasterReadData(SMBusMasterTransferSize_1bit) & _BV(NACK_BIT)) {
        // no ack from slave
        return -1;
    }

    return 0;
}

uint8_t
SMBusMasterReadACK(void)
{
    uint8_t data = SMBusMasterReadData(SMBusMasterTransferSize_8bit);
    SMBusMasterWriteData(0x00, SMBusMasterTransferSize_1bit);

    return data;
}

uint8_t
SMBusMasterReadNACK(void)
{
    uint8_t data = SMBusMasterReadData(SMBusMasterTransferSize_8bit);
    SMBusMasterWriteData(0xFF, SMBusMasterTransferSize_1bit);

    return data;
}

void
SMBusMasterStop(void)
{
    PORT_USI &= ~_BV(PORT_USI_SDA);
    PORT_USI |= _BV(PORT_USI_SCL);
    while (!(PIN_USI & _BV(PIN_USI_SCL)));

    _delay_us(T_SU);

    PORT_USI |= _BV(PORT_USI_SDA);

    _delay_us(T_BUF);
}
