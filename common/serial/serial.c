#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "uart.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "led.h"

static struct serial_device *serial_devices = NULL ,*serial_current = NULL;

/* Multi serial device functions */ 
#define DECLARE_STM32_SERIAL_FUNCTIONS(port) \
void stm32_serial##port##_init(uint32_t baudrate) {  stm32_uart##port##_init(baudrate) ; }	\
char stm32_serial##port##_getc(void)   { return stm32_uart##port##_getc() ; }   \
int  stm32_serial##port##_tstc(void)   { return stm32_uart##port##_tstc() ; }  \
void stm32_serial##port##_putc(const char c)  { stm32_uart##port##_putc(c); }  \
void stm32_serial##port##_puts(const char *s) { stm32_uart##port##_puts(s); }  \
void stm32_serial##port##_send(const char *s,int len)  { stm32_uart##port##_send(s,len); }


#define INIT_STM32_SERIAL_STRUCTURE(port, __name ,__baudrate,__console) {   \
	__name,                              \
	__baudrate ,                         \
	__console,                           \
	stm32_serial##port##_init,           \
	NULL ,                               \
	stm32_serial##port##_tstc,           \
	stm32_serial##port##_getc,           \
	stm32_serial##port##_putc,           \
	stm32_serial##port##_puts,           \
	stm32_serial##port##_send,           \
	NULL ,                               \
}
 						
DECLARE_STM32_SERIAL_FUNCTIONS(1) ;
struct serial_device stm32_serial1_device = INIT_STM32_SERIAL_STRUCTURE(1, "serial1", 115200, 1);

#if defined(CONFIG_SERIAL_MULTI) 
INIT_STM32_SERIAL_STRUCTURE(2); 
struct serial_device stm32_serial2_device = 
    INIT_STM32_SERIAL_STRUCTURE(2, "stm32uart2", 115200, 0); 
INIT_STM32_SERIAL_STRUCTURE(3); 
struct serial_device stm32_serial3_device = 
    INIT_STM32_SERIAL_STRUCTURE(3, "stm32uart3", 115200, 0);

#endif 

struct serial_device * default_serial_console(void)
{
    return &stm32_serial1_device ;
}


void serial_devices_register(struct serial_device * dev)
{
    struct serial_device * s = serial_devices ;
    if( s == NULL )
    {
        serial_devices =  dev ;
        return ;
    }
    while( s->next != NULL )
    {
        s = s->next ;
    }
    s->next = dev ;
    s->next->next = NULL ;
}

void serial_devices_init(void )
{
    serial_devices = &stm32_serial1_device ;
    serial_current = &stm32_serial1_device ;
    serial_devices_register(&stm32_serial1_device);
#if defined(CONFIG_SERIAL_MULTI)
    serial_devices_register(&stm32_serial2_device ) ;
    serial_devices_register(&stm32_serial3_device ) ;
#endif
}

void serial_init(unsigned int baudrate )
{
    struct serial_device *dev = serial_current;
    if ( !serial_current) {
        dev = default_serial_console ();
    }
    dev->init(baudrate);
}

/*
 * Test whether a character is in the RX buffer
*/ 
int serial_tstc (void)
{
    struct serial_device *dev = serial_current;
    if ( !serial_current) {
        dev = default_serial_console ();
    }
   return  dev->tstc();
}

void serial_putc(const char c)
{
    struct serial_device *dev = serial_current;
    if ( !serial_current) {
        dev = default_serial_console ();
    }
    if( dev->console && c == '\n')
    dev->putc('\r');	
    dev->putc(c);
}

void serial_puts(const char *s)
{
    struct serial_device *dev = serial_current;
    if ( !serial_current) {
        dev = default_serial_console ();
    }
	while(*s)
	{
	    if( dev->console && *s == '\n')
        {   
            dev->putc('\r');
        }
        dev->putc(*s);
        s++ ;
	}
}

void serial_send(const char *s , int len)
{
    struct serial_device *dev = serial_current;
    if ( !serial_current) {
        dev = default_serial_console ();
    }
    dev->send(s,len);
}


char serial_getc(void )
{
    struct serial_device *dev = serial_current;
    if (!serial_current) {
        dev = default_serial_console ();
    }	
   return  dev->getc();
}

void serial_gets(unsigned char *str)
{
    struct serial_device *dev = serial_current;
    char *str1 = (char *)str;
    char c;
    if(!serial_current) {
        dev = default_serial_console ();
    }	
    if( dev->console )
    {
        while( (c = dev->getc()) != '\r' )
        {       
            if( c == '\b' )                     // backspace  
            {   
                if( (int)str1 < (int)str )
                {
                    //dev->puts("\b \b");       // put char backspace space backspace               
                    serial_puts("\b \b");
                    str--;         
                }   
            }
            else
            {
                *str++ = c;
                dev->putc(c);  
            }
        }
        *str++ = '\0';
        dev->putc('\n');
    }
    else
    {
        *str++ = dev->getc() ;
    }
}

void  serial_printf(const char *format, ...)
{
    char  buffer[255];
    va_list  vArgs;
    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
  	serial_puts(buffer)  ;
}

