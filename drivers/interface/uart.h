#ifndef _UART_H_2
#define _UART_H_2

#include"stdint.h"

#define __DECLARE_STM32_UART_FUNCTIONS(port)  \
 void stm32_uart##port##_init(uint32_t baudrate) ;\
 char stm32_uart##port##_getc(void);    \
 int  stm32_uart##port##_tstc(void);    \
 void stm32_uart##port##_putc(const char c); \
 void stm32_uart##port##_puts(const char* str); \
 void stm32_uart##port##_send(const char *str,int len)


__DECLARE_STM32_UART_FUNCTIONS(1);
__DECLARE_STM32_UART_FUNCTIONS(2);
__DECLARE_STM32_UART_FUNCTIONS(3);


#endif       /*   _UART_H  */

