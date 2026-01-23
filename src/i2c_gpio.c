/**
 * @file i2c_gpio.c
 * @author wwyyy (1046685883@qq.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-22
 * 
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 * 
 */
#include "i2c_gpio.h"
#include "string.h"

// #define MINIMUM_RAM__

/**
 * @brief 
 * 
 */
typedef struct {
#ifdef MINIMUM_RAM__
	const I2cGpio_CfgType *CfgPtr;
#else
	const I2cGpio_CfgType Config;
#endif
} I2cGpio_InfType;

/**
 * @brief 
 * 
 */
static I2cGpio_InfType I2cGpio_Info;

/**
 * @brief 
 * 
 */
#ifdef MINIMUM_RAM__
	#define I2cConfig      (I2cGpio_Info.CfgPtr)
	#define I2cCfg(Member) (I2cConfig->Member)
#else
	#define I2cConfig      (I2cGpio_Info.Config)
	#define I2cCfg(Member) (I2cConfig.Member)
#endif
#define I2cInsCfg(Member) ((I2cCfg(InsCfgPtr) + ins)->Member)
#define I2cInsInf(Member) ((I2cCfg(InsInfPtr) + ins)->Member)

/**
 * @brief 
 * 
 */
#define GPIO_DIR_INPUT  1
#define GPIO_DIR_OUTPUT 0

/**
 * @brief 初始化IIC GPIO状态
 * 
 * @param cfgPtr 
 */
void I2cGpio_Init(const I2cGpio_CfgType *cfgPtr)
{
	uint8 ins;

	memset(&I2cGpio_Info, 0, sizeof(I2cGpio_Info));
	if (NULL == cfgPtr)
		return;
#ifdef MINIMUM_RAM__
	I2cConfig = cfgPtr;
#else
	memcpy((void *)&I2cConfig, cfgPtr, sizeof(I2cGpio_CfgType));
#endif
	// memset((void *)I2cCfg(InsInfPtr), 0, sizeof(I2cGpio_InsInfType) * I2cCfg(InsNum));
	for (ins = 0; ins < I2cCfg(InsNum); ins++) {    // 待确认是否存在I2C死锁的可能!!
		I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
		if (I2cCfg(SDA_ChangeDir_FuncPtr)) {
			I2cCfg(SDA_ChangeDir_FuncPtr)(ins, GPIO_DIR_OUTPUT);
		}
		I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
		// I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_BUF));
	}
}

/**
 * @brief 主机发送一个启动时序
 * 
 * @param ins 
 */
void I2cGpio_Start(uint8 ins)
{
	if (ins >= I2cCfg(InsNum) || NULL == I2cCfg(InsCfgPtr))
		return;
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_BUF));
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, LOW);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HD_STA));
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, LOW);
}


/**
 * @brief 主机向总线发送一个字节数据,并返回应答状态
 * 
 * @param ins 
 * @param wrData 
 * @return uint8 0:未应答 1:应答
 */
uint8 I2cGpio_WrDatChkAck(uint8 ins, uint8 wrData)
{
	uint8 i, status;
	if (ins >= I2cCfg(InsNum) || NULL == I2cCfg(InsCfgPtr))
		return 0;
	for (i = 0; i < 8; i++) {
		I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HD_DAT));
		if (wrData & 0x80) {
			I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
		} else {
			I2cCfg(SDA_SetStatus_FuncPtr)(ins, LOW);
		}
		wrData <<= 1;
		I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_SU_DAT));
		I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
		I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
		I2cCfg(SCL_SetStatus_FuncPtr)(ins, LOW);
	}
	if (I2cCfg(SDA_ChangeDir_FuncPtr))
		I2cCfg(SDA_ChangeDir_FuncPtr)(ins, GPIO_DIR_INPUT);
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HD_DAT));
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	status = I2cCfg(SDA_GetStatus_FuncPtr)(ins);
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, LOW);
	if (I2cCfg(SDA_ChangeDir_FuncPtr))
		I2cCfg(SDA_ChangeDir_FuncPtr)(ins, GPIO_DIR_OUTPUT);
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, status ? HIH : LOW);
	if (status) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * @brief 主机从总线读取一个字节数据
 * 
 * @param ins : 
 * @param ack : 0:不发送应答 1:发送应答
 * @return uint8 : 
 */
uint8 I2cGpio_RdDatSendAck(uint8 ins, uint8 ack)
{
	uint8 i, rdData = 0;
	if (ins >= I2cCfg(InsNum) || NULL == I2cCfg(InsCfgPtr))
		return 0;
	if (I2cCfg(SDA_ChangeDir_FuncPtr))
		I2cCfg(SDA_ChangeDir_FuncPtr)(ins, GPIO_DIR_OUTPUT);
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	for (i = 0; i < 8; i++) {
		I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
		rdData <<= 1;
		I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
		if (I2cCfg(SDA_GetStatus_FuncPtr)(ins)) {
			rdData++;
		}
		I2cCfg(SCL_SetStatus_FuncPtr)(ins, LOW);
		I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	}
	if (I2cCfg(SDA_ChangeDir_FuncPtr))
		I2cCfg(SDA_ChangeDir_FuncPtr)(ins, GPIO_DIR_OUTPUT);
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, ack ? LOW : HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, LOW);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_HIGH));
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);    //
	return rdData;
}

/**
 * @brief 主机产生一个停止时序
 * 
 * @param ins : 
 */
void I2cGpio_Stop(uint8 ins)
{
	if (ins >= I2cCfg(InsNum) || NULL == I2cCfg(InsCfgPtr))
		return;
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, LOW);
	I2cCfg(SCL_SetStatus_FuncPtr)(ins, HIH);
	I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_SU_STO));
	I2cCfg(SDA_SetStatus_FuncPtr)(ins, HIH);
	// I2cCfg(TimDelayUs_FuncPtr)(I2cInsCfg(TIM_BUF));
}
