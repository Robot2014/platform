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

#if(I2C_MODE == STM32_I2C_HW)	         /*  STM32_I2C_HW MODE  */

#include "stm32f10x_i2c.h"
#define  I2C_GPIO_PORT      GPIOB
#define  I2C_PORT           I2C2
#define  I2Cx_SPEED         50000
#define  I2Cx_OWNER_ADDR    0x0A
#define  SCL_PIN            GPIO_Pin_10
#define  SDA_PIN            GPIO_Pin_11

/**
  *@brief       Initialize the Embedded I2C_2 GPIO Interface         
  *@note       This function should be used only after reset.
  *@param     None
  *@return     None
**/
static void I2C_GPIO_Config(I2C_TypeDef* I2Cx,GPIO_TypeDef* GPIOx  )
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

    /* enable I2C GPIO */
    if( GPIOx == GPIOB )
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    if(I2Cx == I2C2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE); 
    }else if(I2Cx == I2C1){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 
    }
    /* PB10---I2Cx_SCL¡¢PB11---I2Cx_SDA*/
    GPIO_InitStructure.GPIO_Pin =  SCL_PIN | SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       
    GPIO_Init(GPIOx, &GPIO_InitStructure);

}

/**
  * @brief     Initialize the Embedded I2C_2 mode  
  * @note     This function should be used only after reset.
  * @param   None
  * @return   None
**/
static void I2C_Mode_Config(I2C_TypeDef* I2Cx  )
{
    I2C_InitTypeDef  I2C_InitStructure; 
    /*     enable I2C_2 power           */
    //     RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx,ENABLE); 
    /*     I2C config */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    //I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWNER_ADDRESS7; 
    //I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2Cx_SPEED;

    /*  enable I2Cx */
    I2C_Cmd(I2Cx, ENABLE);
    /* I2Cx  initialize */
    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_AcknowledgeConfig(I2Cx, ENABLE);   
}

/**
  *@brief	Initialize the Embedded I2C Interface      
  *@note	This function should be used only after reset.
  *@param  	None
  *@return 	None
  */
static void stm32_i2c_init(I2C_TypeDef* I2Cx ,GPIO_TypeDef* GPIOx )
{
    I2C_GPIO_Config(I2Cx,GPIOx);
    I2C_Mode_Config( I2Cx);
}

/**
  * @brief   use I2C Interface to send an buffer to slave device         
  * @note    This function should be used only after reset.
  * @param   None
  * @return  None
  */
static unsigned char stm32_i2c_write_buffer(I2C_TypeDef* I2Cx ,unsigned char device_address, unsigned char write_address,unsigned char *buffer,unsigned char Length)
{
    unsigned char *temp  = buffer ;
    while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY)); 
    /* Send STRAT condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  

    /* Send I2C address for write */
    I2C_Send7bitAddress(I2Cx, device_address, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2Cx, write_address); 				    
           
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
    
    while(Length--)
    {
        /* Send the byte to be written */
        I2C_SendData(I2Cx, *buffer);            
        /* Test on EV8 and clear it */
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        buffer++;
    }
    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
	return *temp;
}

/**
  * @brief   use I2C Interface to read  buffer from slave device  
  * @note    This function should be used only after reset.
  * @param   device_address :read_address
  * @return  None
**/
static unsigned char stm32_i2c_read_buffer(I2C_TypeDef* I2Cx ,unsigned char device_address,unsigned char read_address ,unsigned char *buffer,unsigned char len)
{  
    unsigned char *temp  = buffer ;
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 
        
    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send Devices address for write */
    I2C_Send7bitAddress(I2Cx, device_address, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2Cx, ENABLE);

    /* Send the internal address to write to */
    I2C_SendData(I2Cx, read_address);  

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2Cx, ENABLE);
  
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2Cx, device_address, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while(len)  
    {
        if(len == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2Cx, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the EEPROM */
            *buffer = I2C_ReceiveData(I2Cx);

            /* Point to the next location where the byte read will be saved */
            buffer++; 

            /* Decrement the read bytes counter */
            len--;        
        }   
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
	return *temp;
}

void i2c_init(void)
{
    stm32_i2c_init(I2C_PORT ,I2C_GPIO_PORT );
}
unsigned char i2c_write_buffer(unsigned char device_address, unsigned char write_address,unsigned char *buffer,unsigned char Length)
{
    return stm32_i2c_write_buffer(I2C_PORT ,device_address,write_address,buffer, Length);
}
unsigned char i2c_write_byte(unsigned char device_address,unsigned char write_address, unsigned char write_data)
{
    unsigned char wbyte = write_data ;
    return stm32_i2c_write_buffer(I2C_PORT ,device_address,write_address,&wbyte, 1);
}
unsigned char i2c_read_buffer(unsigned char device_address,unsigned char read_address ,unsigned char *buffer,unsigned char len)
{
    return stm32_i2c_read_buffer(I2C_PORT,device_address,read_address ,buffer,len);
}
unsigned char i2c_read_byte(unsigned char device_address,unsigned char read_address )
{
    unsigned char read_data ;
    stm32_i2c_read_buffer(I2C_PORT,device_address,read_address ,&read_data,1);
    return  read_data;
}

#endif 