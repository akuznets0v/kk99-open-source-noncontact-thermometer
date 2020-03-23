/**
 * Initialize MLX90614 infrared sensor.
 *
 **/
extern void
MLXIRSensorInit(void);

/**
 * Read object temperature in degrees of celsius from IR sensor.
 *
 * @return temperature in degrees of celsius
 **/
extern float
MLXIRSensorReadTemperature(void);
