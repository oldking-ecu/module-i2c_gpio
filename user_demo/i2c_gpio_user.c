/**
 * @file i2c_gpio_user.c
 * @author wwyyy (1046685883@qq.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-22
 * 
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 * 
 */
#include "i2c_gpio_user.h"
#include "mcu_hal_user.h"
#include "public.h"

static void SCL(uint8 ins, uint8 level)
{
	McuDio_SetStatus(MCU_DIO_O_INA_SCL + ins, level);
}
static void SDA(uint8 ins, uint8 level)
{
	McuDio_SetStatus(MCU_DIO_O_INA_SDA + ins, level);
}
static void SET_SDA_DIR(uint8 ins, uint8 isInput)
{
	if (isInput) {
		GPIOB->cfghr_bit.iomc11 = 0;
	} else {
		GPIOB->cfghr_bit.iomc11 = 2;
	}
	// mcu_dio_change_mode(MCU_DIO_O_INA_SDA+ins, isInput ? DI_PU : DO_PP);
}
static uint8 GET_SDA(uint8 ins)
{
	return McuDio_GetStatus(MCU_DIO_O_INA_SDA + ins);
}

/**
 * @brief 
 * 
 */
static const I2cGpio_InsCfgType I2cGpio_InsCfg[] = {
	{.TIM_HIGH = 1, .TIM_SU_STA = 0, .TIM_HD_STA = 0, .TIM_SU_DAT = 1, .TIM_HD_DAT = 0, .TIM_SU_STO = 0, .TIM_BUF = 1}, //  200K
	{.TIM_HIGH = 1, .TIM_SU_STA = 0, .TIM_HD_STA = 0, .TIM_SU_DAT = 1, .TIM_HD_DAT = 0, .TIM_SU_STO = 0, .TIM_BUF = 1}, //  250K
};

/**
 * @brief 
 * 
 */
void I2cGpio_UserInit(void)
{
	I2cGpio_CfgType I2cGpio_Config = {0};
	I2cGpio_Config.InsCfgPtr = I2cGpio_InsCfg;
	I2cGpio_Config.InsNum = ARRAY_SIZE(I2cGpio_InsCfg);
	I2cGpio_Config.SCL_SetStatus_FuncPtr = SCL;
	I2cGpio_Config.SDA_SetStatus_FuncPtr = SDA;
	I2cGpio_Config.SDA_GetStatus_FuncPtr = GET_SDA;
	I2cGpio_Config.TimDelayUs_FuncPtr = Mcu_SoftDelayus;
	// I2cGpio_Config.SDA_ChangeDir_FuncPtr = SET_SDA_DIR;
	I2cGpio_Init(&I2cGpio_Config);
}
