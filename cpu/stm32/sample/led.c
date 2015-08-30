#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#define LED_OFF     GPIOB->BSRR |= GPIO_Pin_8
#define LED_ON      GPIOB->BRR  |= GPIO_Pin_8
#define LED_FLASH   GPIOB->ODR  ^= GPIO_Pin_8
void led_init(void)
{
    GPIO_InitTypeDef GP_B;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   
    GP_B.GPIO_Pin=GPIO_Pin_8 ;
    GP_B.GPIO_Mode=GPIO_Mode_Out_PP;
    GP_B.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GP_B);	  
}

void led_on(void)
{
    LED_ON ;
}

void led_off(void)
{
    LED_OFF ;
}

void led_flash(void)
{
    LED_FLASH ;
}

int main(void)
{
   int	i = 0 ;
	led_on() ;
	for ( i =0 ; i< 500 ; i++)
	led_off();
	for(i = 0 ; i < 500 ; i++ )
	led_on();
	while(1);
    return 0 ;
}
