/*
 * (C) Copyright 2014  ShenZhen China
 *  Jundi.chen Software Engineering, <chenjundi0808@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 CHINA
 */


#include"config.h"
#include"i2c.h"
#include"led.h"
#include"stdio.h"

#if( I2C_MODE == I2C_SOFTWARE )

#include"stm32f10x_gpio.h"

#define false 0
#define true  1

#define SCL_H         GPIOB->BSRR = SCL_PIN /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOB->BRR  = SCL_PIN /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOB->BSRR = SDA_PIN /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOB->BRR  = SDA_PIN /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL           GPIOB->IDR  & SCL_PIN /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA           GPIOB->IDR  & SDA_PIN /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

#endif             


#if( I2C_MODE == I2C_SOTEWARE )

/**
  * @brief   genarate I2C time delay 
  * @param   None
  * @return  None
  */
static void I2C_delay(void)
{
   int i = 30;
    while (i)
    i--;
}
/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief   Initialize the Embedded I2C Interface      
  * @note    This function should be used only after reset.
  * @param   None
  * @return  None
  */
void i2c_init(void)
{
	I2C_GPIO_Config();
}


/**
  * @brief   genarate I2C start condition 
  * @param   None
  * @return  None
  */
static int8_t I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if (!SDA)               
		return false;   
    SDA_L;
    I2C_delay();
    if (SDA)            	
		return false;
    SDA_L;
    I2C_delay();
    return true;
}

/**
  * @brief   genarate I2C stop condition 
  * @param   None
  * @return  None
  */
static void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}
/**
  * @brief   genarate I2C ACK signal 
  * @param   None
  * @return  None
  */
static void I2C_Ack(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static void I2C_NoAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}
/**
  * @brief   wait ack signal from I2C bus 
  * @param   None
  * @return  None
  */
static int32_t I2C_WaitAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if (SDA) 
	{
        SCL_L;
		I2C_delay();
        return false;
    }
    SCL_L;
	I2C_delay();
    return true;
}

/**
  * @brief   send 8bit data by I2C  
  * @param   byte 8bit send data
  * @return  None
  */
static void I2C_SendByte(unsigned char byte)
{
    unsigned char i = 8;
    while (i--) {
        SCL_L;
        I2C_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}

 /**
  * @brief   get 8bit data from I2C  
  * @param   None
  * @return  8bit data
  */
static unsigned char I2C_ReceiveByte(void)
{
    unsigned char i = 8;
    unsigned char byte = 0;

    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if (SDA) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

/**
  * @brief      
  * @param   device_addresswrite_address  data:
  * @return  None
  */
unsigned char i2c_write_byte(unsigned char device_address,unsigned char write_address, unsigned char write_data)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(device_address);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(write_address);
    I2C_WaitAck();
    I2C_SendByte(write_data);
    I2C_WaitAck();
    I2C_Stop();
	delay5ms();
    return true;
}

/**
  * @brief   send buffer data to slave device  
  * @param   device_address 
  * @return  None
**/
unsigned char i2c_write_buffer(unsigned char device_address, unsigned char write_address,unsigned char *buffer,unsigned char Length)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(device_address);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(write_address );
    I2C_WaitAck();
  
	while(Length--)
	{ 
		I2C_SendByte(*buffer);
		if (!I2C_WaitAck()){
            I2C_Stop();
            return false;
        }
		buffer++;

	}
    I2C_Stop();
    return true;
}

/**
  * @brief        
  * @param   device_address 
  * @return  None
  */
unsigned char i2c_read_buffer(unsigned char device_address, unsigned char Raddr , unsigned char *buffer, unsigned char len)
{
    if (!I2C_Start())
    return false;
    I2C_SendByte(device_address );
    if (!I2C_WaitAck()) 
    {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(Raddr);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(device_address |0x1);
    I2C_WaitAck();
    while (len) {
        *buffer = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buffer++;
        len--;
    }
    I2C_Stop();
    return true;
}

/**
  * @brief	revceive data from slave device 
  * @param  device_address 
  * @return read byte  
  */
unsigned char i2c_read_byte(unsigned char device_address, unsigned char Raddr )
{
   unsigned char Rbyte;
   i2c_read_buffer(device_address, Raddr , &Rbyte, 1);
   return Rbyte;
}

#endif             
