#include "bmi2.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define BMI270_ADDR  (0x69 << 1)

int8_t bmi2_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    if (HAL_I2C_Mem_Read(&hi2c1,
                         (0x69 << 1),
                         reg_addr,
                         I2C_MEMADD_SIZE_8BIT,
                         data,
                         len,
                         1000) != HAL_OK)
        return -1;

    return 0;
}

int8_t bmi2_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    I2C_HandleTypeDef *i2c = (I2C_HandleTypeDef*)intf_ptr;

    while (len > 0)
    {
        uint8_t chunk = (len > 8) ? 8 : len;   // 🔥 limit

        if (HAL_I2C_Mem_Write(i2c,
                              (0x69 << 1),
                              reg_addr,
                              I2C_MEMADD_SIZE_8BIT,
                              (uint8_t*)data,
                              chunk,
                              1000) != HAL_OK)
        {
            return -1;
        }

        data += chunk;
        len  -= chunk;

        HAL_Delay(1);   // 🔥 REQUIRED
    }

    return 0;
}

void bmi2_delay_us(uint32_t period, void *intf_ptr)
{
    if (period < 1000)
        HAL_Delay(1);
    else
        HAL_Delay(period / 1000);
}
