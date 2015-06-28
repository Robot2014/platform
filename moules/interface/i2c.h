/**
 * file name : 	i2c.c
				PB10  ---------------------   SENSOR_SCK
				PB11  ---------------------   SENSOR_SDA
**/
#include "i2c.h"

#define I2C_SPEED_4M            400000  
#define I2C_SPEED_2M            200000   
#define I2C2_OWN_ADDRESS7      	0x0A	 

/**
  * @brief   Initialize the I2C GPIO config
  * @note    This function config i2c gpio mode
  * @param   None
  * @return  None
  */
static void i2c_gpio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C1 有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  

	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief   Initialize the I2C mode config
  * @note    This function config i2c mode
  * @param   speed
  * @return  None
  */
static void i2c_mode_config(int speed)
{
	I2C_InitTypeDef  I2C_InitStructure; 

	/* I2C mode config */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 =I2C2_OWN_ADDRESS7; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;   // 7bit slave addr mode
	I2C_InitStructure.I2C_ClockSpeed = speed;

	/*enable I2C2 */
	I2C_Cmd(I2C2, ENABLE);

	/* I2C2 initialize */
	I2C_Init(I2C2, &I2C_InitStructure);
}

/**
  * @brief   Initialize the Embedded I2C Interface      
  * @note    This function should be used only after reset.
  * @param   speed: i2c speed set ,slaveadd : slave device add
  * @return  None
  */
void i2c_init (int speed)
{
	i2c_gpio_config(); 
	i2c_mode_config(speed);
}

/**
  * @brief   Initialize the Embedded I2C Interface      
  * @note    This function should be used only after reset.
  * @param   device_addr:  slave device address . write_addr: write addr . dat : data
  * @return  None
 **/
void i2c_write_byte(unsigned char device_addr, unsigned char write_addr, unsigned char dat )
{
	/* Send STRAT condition */
	I2C_GenerateSTART(I2C2, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  

	/* Send slave device address for write */
	I2C_Send7bitAddress(I2C2, device_addr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  
	/* Send the device internal address to write to */    
	I2C_SendData(I2C1, write_addr);  

	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 
	
	/* Send the byte to be written */
	I2C_SendData(I2C2, dat); 

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STOP condition */
	I2C_GenerateSTOP(I2C2, ENABLE);
}

/**
  * @brief   write the Embedded I2C Interface      
  * @note    This function should be used only after reset.
  * @param   device_addr:  slave device address . write_addr: write addr . dat : data
  * @return  None
 **/
void i2c_write_buffer(unsigned char device_addr, unsigned char read_addr, unsigned char *buffer, unsigned char buffer_size)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008

	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, device_addr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	/* While there is data to be written */
	while( buffer_size-- )  
	{
		/* Send the current byte */
		I2C_SendData(I2C2, *buffer); 

		/* Point to the next byte to be written */
		buffer++; 

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
  * @brief   write the Embedded I2C Interface      
  * @note    This function should be used only after reset.
  * @param   device_addr:  slave device address . write_addr: write addr . dat : data
  * @return  None
 **/
void i2c_read_buffer(unsigned char device_addr, unsigned char read_addr,unsigned char *buffer, unsigned char buffer_size)
{  
	//*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
		
	/* Send START condition */
	I2C_GenerateSTART(I2C2, ENABLE);
	//*((u8 *)0x4001080c) &=~0x80;

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C2, ENABLE);

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(I2C2, read_addr);  

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C2, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(buffer_size)  
	{
		if( buffer_size == 1 )
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C2, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C2, ENABLE);
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
		  /* Read a byte from the EEPROM */
		  *pBuffer = I2C_ReceiveData(I2C2);

		  /* Point to the next location where the byte read will be saved */
		  buffer++; 
		  
		  /* Decrement the read bytes counter */
		  buffer_size--;        
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**
  * @brief   i2c_read_byte      
  * @note    This function should be used only after reset.
  * @param   device_addr:  slave device address . write_addr: write addr . dat : data
  * @return  None
 **/
void i2c_read_byte(unsigned char device_addr, unsigned char read_addr)
{
	unsigned char Rbyte;
	i2c_Rbuffer(device_addr, read_addr , &Rbyte, 1);
	return Rbyte;
}

