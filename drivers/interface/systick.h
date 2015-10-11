#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

#define TICK_CLOCK    72000000 
//#define     TICK_CLOCK    9000000  
#define HZ  1000

#define typecheck(__type,x)     1
/*
({  __type __dummy; \
    typeof(x) __dummy2; \
    (void)(&__dummy == &__dummy2); \
    1;\
})
*/

#define time_after(a,b) \
(typecheck(unsigned int, a) && \
 typecheck(unsigned int, b) && \
((int)(b) - (int)(a) < 0)) 

/* a before b */
#define time_before(a,b) time_after(b,a)

#define time_after_eq(a,b) \
(typecheck(unsigned int, a) && \
typecheck(unsigned int, b) && \
((int)(a) - (int)(b) >= 0))
#define time_before_eq(a,b) time_after_eq(b,a)

void systick_init(void);
uint32_t get_current_time(uint32_t *current_time) ;
uint32_t get_diff_time(uint32_t *pre_time);

void delay( uint32_t s) ;
void mdelay( uint32_t ms);
void udelay( uint32_t us);

#endif /* __SYSTICK_H */
