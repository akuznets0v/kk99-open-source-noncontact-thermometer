#include <MLXIRSensor.h>
#include <SMBusMaster.h>

#define MLX90614_DEVICE_ADDRESS 0x5A

void
MLXIRSensorInit(void)
{
    SMBusMasterInit();
}

float
MLXIRSensorReadTemperature(void)
{
    float temperature = 0;
    uint8_t deviceAddress = MLX90614_DEVICE_ADDRESS << 1;
    uint16_t data = 0;
    int8_t status = 0;

    status = SMBusMasterStart(deviceAddress + SMBUS_WRITE);
    if (status < 0) {
        goto err;
    }

    status = SMBusMasterWrite(0x07);
    if (status < 0) {
        goto err;
    }

    status = SMBusMasterRepeatedStart(deviceAddress + SMBUS_READ);
    if (status < 0) {
        goto err;
    }

    data = SMBusMasterReadACK();
    data |= SMBusMasterReadACK() << 8;
    SMBusMasterReadNACK();
    SMBusMasterStop();

    temperature = data * 0.02;
    temperature -= 273.15;

    return temperature;

err:
    SMBusMasterStop();
    return temperature;
}
