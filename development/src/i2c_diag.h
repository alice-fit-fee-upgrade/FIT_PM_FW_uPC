#ifndef PM_I2C_DIAG_H
#define PM_I2C_DIAG_H
#include <stdint.h>
enum { PM_I2C_OK, PM_I2C_BUSY, PM_I2C_NACK, PM_I2C_TIMEOUT, PM_I2C_RESET, PM_I2C_STUCK, PM_I2C_RESTORE, PM_I2C_ID, PM_I2C_POWER };
typedef struct { uint16_t raw[6],configuration;uint8_t mask70,mask71; } pm_ina_sample;
uint8_t pm_ina_read(uint8_t route,pm_ina_sample *sample);
int16_t pm_ina_code(uint16_t raw);
void pm_i2c_delay(void);
#endif
