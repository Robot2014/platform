#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "misc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "uart.h"

#define CONFIG_SERIAL_SOFTWARE_FIFO

#ifdef  CONFIG_SERIAL_SOFTWARE_FIFO 
#define UART_BUF_SIZE   64
struct st_uart_buf
{
    char rx_buf[UART_BUF_SIZE] ;
    char tx_buf[UART_BUF_SIZE] ;
    unsigned char rx_count ;
    unsigned char tx_count ;
};

struct st_uart_buf uart_buf ;

#endif 

#define DECLARE_STM32_UART_FUNCTIONS(port) \
void stm32_uart##port##_init(uint32_t baudrate)          {\
	 stm32_uart##port##_gpio_init();                      \
     stm32_uart_mode_init(USART##port##,baudrate) ;       \
     stm32_uart_nvic_cfg(USART##port##_IRQn,ENABLE); }    \
char stm32_uart##port##_getc(void)  {return stm32_uart_getc(USART##port##) ;}   \
int  stm32_uart##port##_tstc(void)  {return stm32_uart_getc(USART##port##) ;}   \
void stm32_uart##port##_putc(const char c){ stm32_uart_putc(USART##port##,c);}  \
void stm32_uart##port##_puts(const char* str) { stm32_uart_puts(USART##port##,str); } \
void stm32_uart##port##_send(const char*str, int len){ stm32_uart_send(USART##port##,str,len) ;}

static void stm32_uart1_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;          
    /* config USART1 clock */   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);    
	/* USART1 GPIO config */   
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);        
	/* Configure USART1 Rx (PA.10) as input floating */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);             
}

static void stm32_uart2_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;          
    /* config USART1 clock */   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);    
	/* USART1 GPIO config */   
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);        
	/* Configure USART1 Rx (PA.03) as input floating */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);             
}

static void stm32_uart3_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;          
    /* config USART3 clock */   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);    
	/* USART1 GPIO config */   
	/* Configure USART1 Tx (PB.10) as alternate function push-pull */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);        
	/* Configure USART1 Rx (PB.11) as input floating */   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);             
}

static void stm32_uart_mode_init(USART_TypeDef* USARTx ,uint32_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;
    if(USARTx == USART1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
    if(USARTx == USART2)
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    if(USARTx == USART3)
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);   
	 /* USART1 mode config */   
	USART_InitStructure.USART_BaudRate = baudrate;    
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    
	USART_InitStructure.USART_Parity = USART_Parity_No ;    
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   
	USART_Init(USARTx, &USART_InitStructure);     

	USART_Cmd(USARTx, ENABLE); 
    USART_ClearFlag(USARTx£¬USART_FLAG_TC); // clean tc flag for firstr char error
}

static void stm32_uart_nvic_cfg (IRQn_Type uart_irqn  ,FunctionalState irq_cfg)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = uart_irqn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = irq_cfg;
	NVIC_Init(&NVIC_InitStructure);
}

static void stm32_uart_putc(USART_TypeDef* USARTx ,const char c)
{
#if   CONFIG_SERIAL_SOFTWARE_FIFO 
    uart_buf.tx_buf[0] = c ;
    uart_buf.tx_count = 1 ;
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
#else
	USART_SendData(USARTx,(unsigned char)c);
	while (!(USARTx->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET);   
#endif 
}


static void stm32_uart_puts(USART_TypeDef* USARTx ,const char* str )
{
    int len = strlen(str) ;
    do{
        uart_buf.tx_count = len > UART_BUF_SIZE ? UART_BUF_SIZE : len ;
        memcpy(uart_buf.tx_buf , str , uart_buf.tx_count );
        len -= UART_BUF_SIZE ;
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    }while( len > UART_BUF_SIZE );
    
}

static void stm32_uart_send(USART_TypeDef* USARTx ,const char* str ,int len)
{
    int i = 0 , len1 = 0 ;
    char *str1 =(char *)str ;
    for (i = 0 ; i < len/UART_BUF_SIZE ; i++)
    {
        while( uart_buf.tx_count == 0 );
        len1 = (len > UART_BUF_SIZE) ? UART_BUF_SIZE : len ;
        memcpy(uart_buf.tx_buf , str1 , len1 );
        uart_buf.tx_count = len1 ;
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
        len -= UART_BUF_SIZE ;
        str1 += len1 ;
    }
}

static char  stm32_uart_getc( USART_TypeDef* USARTx  ) 
{
#ifdef  CONFIG_SERIAL_SOFTWARE_FIFO 

    while( uart_buf.rx_count == 0 ) ;
    return uart_buf.rx_buf[uart_buf.rx_count--] ; 

#else  
    while (!(USARTx->SR & USART_FLAG_RXNE));
	return USART_ReceiveData(USARTx) ;
#endif
}

/*
 * Test whether a character is in the RX buffer
 */
char stm32_uart_tstc( USART_TypeDef* USARTx )
{
    return (USARTx->SR & USART_FLAG_RXNE) ;
}

void stm32_uart0_interrupt( void )
{   
    static unsigned char tx_point = 0 ;
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{	  
	    
		USART_SendData(USART1,uart_buf.tx_buf[ tx_point++ ]  ); 
		while (!(USART1->SR & USART_FLAG_TXE));
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
        if( tx_point == uart_buf.tx_count)  // tx buff is end
		{		
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            uart_buf.tx_count = 0;
            tx_point = 0 ;
		}
	}

    else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
    {
		 uart_buf.rx_buf[ uart_buf.rx_count++] = USART_ReceiveData(USART1) ;
         if(uart_buf.rx_count == UART_BUF_SIZE)
         uart_buf.rx_count = 0;
    }
    
}  
int fputc(int ch, FILE *f)
{
	if( ch == '\n' )
	{
		USART_SendData(USART1,'\r');
		while (!(USART1->SR & USART_FLAG_TXE));
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET); 
	}
	USART_SendData(USART1, (unsigned char) ch);
    while (!(USART1->SR & USART_FLAG_TXE));
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	return (ch);
}

DECLARE_STM32_UART_FUNCTIONS(1)	 ;
DECLARE_STM32_UART_FUNCTIONS(2)	 ;
DECLARE_STM32_UART_FUNCTIONS(3)	 ;

