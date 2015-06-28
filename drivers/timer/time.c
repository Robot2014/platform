/**
  ******************************************************************************
  * @file    time.c
  * @author  Corner Boy   QQ:251534538  mail:chenjunyu@hodmct.com
  * @version V1.0
  * @date    14-April-2013
  * @brief   This c file is used to  
  * 
  * @dagram	  
              stm32 ------I2C------mpu6050-------hml8553L 
  ******************************************************************************
  * @attention
  * This file is free for all who is crazy of quadcopter_X4 
  * if you find and bug you can communication to me by QQ or email.
  * Thanks
  ******************************************************************************
**/
#include"stm32f10x_tim.h"
#include"led.h"
#include"stm32f10x.h"
#include "stm32f10x_it.h"

#include"misc.h"

/**
  * @brief   time1 init  
  * @param   int_time 中断时间	 单位ms
  * @retval  None
**/
void time1_init(int int_time)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	  	
	NVIC_InitTypeDef NVIC_InitStructure; 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler=(3600-1);         		//时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  		// 时钟分割
	TIM_TimeBaseStructure.TIM_Period=20*int_time; 			 			//定时器初始值
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);   			//初始化定时器的值
    TIM_ClearFlag(TIM1,TIM_FLAG_Update);               			//清除定时器中断标志  
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  					//使能中断
    TIM_Cmd(TIM1,ENABLE);            							//开启时钟

	 /*nvic*/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
     
}


void TIM1_IRQHandler(void)
{
	  if ( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) {
         TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);//必须清除中断标志位否则一直中断
    }
	 LED_FLASH;

}  


void TIME_Config(TIM_TypeDef* TIMx,int int_time)
 {
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;
	if(TIMx==TIM2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  
	}
	if(TIMx==TIM3)		 
	{
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	}
	if(TIMx==TIM4)
	{
   		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	}
   	TIM_DeInit(TIMx);            								//复位定时器
   	TIM_TimeBaseStructure.TIM_Prescaler=(3600-1);         		//时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  		// 时钟分割
	TIM_TimeBaseStructure.TIM_Period=20*int_time; 			 	//定时器初始值
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
    TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);   			//初始化定时器的值
    TIM_ClearFlag(TIMx,TIM_FLAG_Update);               			//清除定时器中断标志  
    TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);  					//使能中断
    TIM_Cmd(TIMx,ENABLE);            							//开启时钟

	 /*nvic*/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
	if(TIMx==TIM2)
	{
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	}
	if(TIMx==TIM3)		 
	{
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	}
	if(TIMx==TIM4)
	{
   	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

 }

void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{
         TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);			//必须清除中断标志位否则一直中断
    }
   	 LED_FLASH;
}


void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
         TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);			//必须清除中断标志位否则一直中断
    }
   	 LED_FLASH;
}


void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{
         TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);			//必须清除中断标志位否则一直中断
    }
   	 LED_FLASH;
}

