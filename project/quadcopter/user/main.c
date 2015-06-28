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
#include"uart.h"
#include"time.h"
#include"stm32f10x_gpio.h"
#include"sensor.h"
#include"mpu6050.h"
#include"nRF24L01.h"
#include"quadcopter.h"
#include<string.h>

/* static void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}		*/

uint8_t rxbuf[4];
int main(void)
{  
	int status;
	memset(RX_BUF,0,4);
	led_init();
	nRF24L01_init();
	motor_init();			//电机初始化
	QuadCopter_init(&QuadCopter);
	uart_init(115200);
	
   	status = nRF_Check(); 	   	 	/*检测NRF模块与MCU的连接*/
   if(status == SUCCESS)			/*判断连接状态*/  	   
   		 uart_printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	uart_printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");
	nRF_RX_Mode();
	LED_OFF;	 	
while(1)
{

	nRF_RX_Mode();
	nRF_Rx_Dat(RX_BUF);
	status=RX_BUF[2];
	
   switch(status)
   {
	 case Q_ON:
	   LED_ON;
	   QuadCopter.Status=Q_ON;
	   QuadCopter.BaseSpeed=300;
	  motor_speed(QuadCopter.BaseSpeed,QuadCopter.BaseSpeed,QuadCopter.BaseSpeed,QuadCopter.BaseSpeed ) ;
	   break;
	  case Q_UP:
	  	QuadCopter_up(&QuadCopter);
		break;
	  case Q_DOWN:
	   	QuadCopter_down(&QuadCopter);
		break;
	   case Q_OFF:
	    LED_OFF;
		QuadCopter.Status=Q_OFF;
	  	motor_speed(0,0,0,0 ) ;
		break;
	 default:
	 break;
   }	 
 }
	  	
}
		


										 

