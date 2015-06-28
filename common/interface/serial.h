#ifndef _UART_H
#define _UART_H

#define NAMESIZE 16
struct serial_device {
	char name[NAMESIZE];
	int  baudrate;
    int  console ;
	void (*init) (unsigned int baudrate);
	void (*setbrg) (void);
	int  (*tstc) (void);  /*Test whether a character is in the RX buffer */
    char (*getc) (void);
	void (*putc) (const char c);
	void (*puts) (const char *s);
    void (*send) (const char *s , int len);
	struct serial_device *next;
};

void serial_devices_init(void ) ;
void serial_init(unsigned int boadrate);
void serial_putc(const  char c);
void serial_puts(const char *s);
void serial_send(const char *s ,int len) ;
void serial_printf(const char *format, ...);
char serial_getc(void);
void serial_gets(unsigned char *str);

#endif       /*   _UART_H  */

