/**
  ******************************************************************************
  * @file    main.c
  * @author  Corner Boy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    14-April-2013
  * @brief   This C file is a main application. 
  * @dagram	  
  ******************************************************************************
  * @attention
  * This file is free for all who is crazy of quadcopter_X4 
  * if you find and bug you can communication to me by QQ or email.
  * Thanks
  ******************************************************************************
**/

#include"stm32f10x.h"
#include"led.h"
#include"time_pwm.h"
#include"motor.h"

#include "uart2.h"
#include"time.h"
#include"stm32f10x_gpio.h"
#include"sensor.h"
#include"mpu6050.h"
#include"nRF24L01.h"
#include"quadcopter.h"
#include<string.h>
#include"stdio.h"
#include"uart.h"
#include"ringbuffer.h"
static void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}		
 extern  ST_RINGBUFFER  tx_ringbuf	;
uint8_t rxbuf[128];
int main(void)
{  
	memset(RX_BUF,0,4);
	led_init();
	nRF24L01_init();
	motor_init();			//电机初始化
	QuadCopter_init(&QuadCopter);

	Delayms(10);
	serial_init(115200);
	 
	LED_ON; 
   	serial_printf("\nhhhhhahhahahha终于可以了  \n ");
//	Delayms(100);
//    serial_flush(&tx_ringbuf);
	serial_putc('o');
	serial_gets((char*)rxbuf);
		
	while(1)
	{
	 
	}
	  	
}
		


										 

