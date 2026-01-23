/**
 * @file i2c_gpio_user.h
 * @author wwyyy (1046685883@qq.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-22
 * 
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 * 
 */
#ifndef I2C_GPIO_USER_H__
#define I2C_GPIO_USER_H__
#include "i2c_gpio.h"

/**
 * @brief 
 * 
 */
typedef enum {
	I2C_GPIO_INS_INA226 = 0,
	I2C_GPIO_INS_SC8815,
	I2C_GPIO_INS_TOTAL_NUM
} I2C_GPIO_INS_ENUM_TYPE;

/**
 * @brief 
 * 
 */
void I2cGpio_UserInit(void);

#endif    // I2C_GPIO_USER_H__
