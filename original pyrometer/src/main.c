#include <SSNIndicator.h>
#include <MLXIRSensor.h>
#include <util/delay.h>

int main(void)
{
    float temperature = 0;
    SSNIndicatorInit();
    MLXIRSensorInit();

    while (1) {
        temperature = MLXIRSensorReadTemperature();
        SSNIndicatorDisplayNumber((int)(temperature * 10.0), 2);
    }

    return 0;
}
