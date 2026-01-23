/**
 * @file i2c_gpio.h
 * @author wwyyy (1046685883@qq.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-22
 * 
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 * 
 */
#ifndef I2C_GPIO_H__
#define I2C_GPIO_H__
#include "type.h"

/**
 * @brief 
 * 
 */
typedef struct {
	uint8 TIM_HIGH;      // 时钟高电平时间
	uint8 TIM_SU_STA;    // SCL高电平到SDA低电平最小时间(START信号前半)
	uint8 TIM_HD_STA;    // SDA拉低到SCL拉低最小时间(START信号后半)
	uint8 TIM_SU_DAT;    // SDA变化后到SCL上升沿最小时间
	uint8 TIM_HD_DAT;    // SCL下降沿到SDA开始变化最小时间
	uint8 TIM_SU_STO;    // SCL拉高到SDA拉高时间
	uint8 TIM_BUF;       // 总线释放或可以下一次START的时间
} I2cGpio_InsCfgType;

/**
 * @brief 
 * 
 */
typedef struct {
	uint16 Dummy1;
} I2cGpio_InsInfType;

/**
 * @brief 
 * 
 */
typedef struct {
	const I2cGpio_InsCfgType *InsCfgPtr;
	// I2cGpio_InsInfType *InsInfPtr;
	uint8 InsNum;
	void (*SCL_SetStatus_FuncPtr)(uint8 ins, uint8 level);
	void (*SDA_SetStatus_FuncPtr)(uint8 ins, uint8 level);
	uint8 (*SDA_GetStatus_FuncPtr)(uint8 ins);
	void (*TimDelayUs_FuncPtr)(uint32 us);
	void (*SDA_ChangeDir_FuncPtr)(uint8 ins, uint8 isInput);    // 可选
} I2cGpio_CfgType;


// 对外提供接口
void I2cGpio_Init(const I2cGpio_CfgType *cfgPtr);
void I2cGpio_Start(uint8 ins);
uint8 I2cGpio_WrDatChkAck(uint8 ins, uint8 data);
uint8 I2cGpio_RdDatSendAck(uint8 ins, uint8 ack);
void I2cGpio_Stop(uint8 ins);


#endif    // I2C_GPIO_H__
