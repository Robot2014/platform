/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Corner Boy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    14-April-2013
  * @brief   This is an C head file about I2C hardware operation . 
  * @dagram	  
				PB10  ---------------------   SENSOR_SCK
				PB11  ---------------------   SENSOR_SDA
  ******************************************************************************
  * @attention
  * This file is free for all who is crazy of quadcopter_X4 
  * if you find any bug or misunderstand you could communication with me by QQ or email.
  * Thanks
  ******************************************************************************
**/



#ifndef          _I2C_H
#define          _I2C_H

#include "config.h"
#include "stdint.h"

#if(I2C_MODE==I2C_SIMULATE)
#include "stm32f10x.h"

#define false 0
#define true  1

#define SCL_PIN GPIO_Pin_10
#define SDA_PIN GPIO_Pin_11

#define SCL_H         GPIOB->BSRR = SCL_PIN /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOB->BRR  = SCL_PIN /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOB->BSRR = SDA_PIN /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOB->BRR  = SDA_PIN /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL           GPIOB->IDR  & SCL_PIN /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA           GPIOB->IDR  & SDA_PIN /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

#endif             

void I2C_GPIO_Config(void);
void I2C_Mode_Config(void);
void i2c_init(void);
unsigned char i2c_Wbyte(unsigned char DevAddr,unsigned char Write_Addr, unsigned char Wdata);
unsigned char i2c_Wbuffer(unsigned char DevAddr, unsigned char Write_Addr,unsigned char *buffer,unsigned char Len);
uint8_t i2c_Rbuffer(uint8_t DevAddr, uint8_t Raddr , uint8_t *buffer, uint8_t lenth) ;
uint8_t i2c_Rbyte(uint8_t DevAddr, uint8_t Raddr );
 void Delayms(vu32 m);


#endif              /*        _I2C_H       */

