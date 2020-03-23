#include <stdint.h>

// SMBus data direction bit
#define SMBUS_WRITE 0
#define SMBUS_READ  1

/**
 * Initialize SMBus master interface.
 **/
extern void
SMBusMasterInit(void);

/**
 * Send start condition, address and transfer direction.
 *
 * @param[in] address address and transfer direction of SMBus device
 * @return            @c 0 on success, otherwise @c -1
 **/
extern int8_t
SMBusMasterStart(uint8_t address);

/**
 * Send repeated start condition, address and transfer direction.
 *
 * @param[in] address address and transfer direction of SMBus device
 * @return            @c 0 on success, otherwise @c -1
 **/
extern int8_t
SMBusMasterRepeatedStart(uint8_t address);

/**
 * Send a byte to SMBus device.
 *
 * @param[in] data data to be transfered
 * @return         @c 0 on success, otherwise @c -1
 **/
extern int8_t
SMBusMasterWrite(uint8_t data);

/**
 * Read one byte from SMBus device.
 * Read is followed by ACK bit.
 *
 * @return byte read from SMBus device
 **/
extern uint8_t
SMBusMasterReadACK(void);

/**
 * Read one byte from SMBus device.
 * Read is followed by NACK bit.
 *
 * @return bute read from SMBus device
 **/
extern uint8_t
SMBusMasterReadNACK(void);

/**
 * Send stop condition and release SMBus bus.
 **/
extern void
SMBusMasterStop(void);
