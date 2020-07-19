#pragma once

#if defined(ENABLE_INA2XX)

typedef struct ina2xx_data {
    int32_t vin;
    int32_t vout;
    int32_t iout;
    int32_t vbat;
} ina2xx_data;

void ina2xx_init();
void get_ina2xx_data(ina2xx_data *sensor_data);
#endif

