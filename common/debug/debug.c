#include"stm32f10x_usart.h"
#include"stdio.h"
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
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
