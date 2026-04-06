#include "BMI2.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

#define BMI270_I2C_ADDR  (0x68 << 1)

int8_t bmi2_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t buffer[32];

    buffer[0] = reg_addr;
    for (uint32_t i = 0; i < len; i++)
    {
        buffer[i + 1] = data[i];
    }

    if (HAL_I2C_Master_Transmit(&hi2c1, BMI270_I2C_ADDR, buffer, len + 1, 100) != HAL_OK)
        return BMI2_E_COM_FAIL;

    return BMI2_OK;
}

int8_t bmi2_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, BMI270_I2C_ADDR, &reg_addr, 1, 100) != HAL_OK)
        return BMI2_E_COM_FAIL;

    if (HAL_I2C_Master_Receive(&hi2c1, BMI270_I2C_ADDR, data, len, 100) != HAL_OK)
        return BMI2_E_COM_FAIL;

    return BMI2_OK;
}
void bmi2_delay_us(uint32_t period, void *intf_ptr)
{
    uint32_t ms = period / 1000;

    if (ms == 0)
        ms = 1;

    HAL_Delay(ms);
}
