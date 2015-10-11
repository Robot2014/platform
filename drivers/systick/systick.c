/*
 * (C) Copyright 2015  ShenZhen China
 *  Jundi.chen Software Engineering, <chenjundi0808@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 CHINA
 */

#include <stdint.h>
#include <stm32f10x.h >
#include "systick.h"

volatile unsigned int jiffies_32 = 0 ;
void systick_init(void)
{
 	SysTick->LOAD = TICK_CLOCK/HZ - 1;   // reload val 24bit FULL 
	SysTick->VAL  = 0x0;
/**
    CTRL bit0  enbale systick 
         bit1  systick interrupt enable 
         bit3  clock source 1: AHB(system main clock)  0: AHB/8
         bit16 count 0 flag
**/
#if(TICK_CLOCK == 72000000)
	SysTick->CTRL = 0xB ;// 1011  
#else
#if(TICK_CLOCK == 9000000 )
    SysTick->CTRL = 0x03;  //0011
#endif
#endif

}   

uint32_t get_current_time(uint32_t *current_time)
{
    *current_time = jiffies_32 ;  // return ms
    return jiffies_32 ;
}

uint32_t get_diff_time(uint32_t *pre_time)
{
    uint32_t cur ;
    return (get_current_time(&cur) - *pre_time );
}

void delay( uint32_t s)
{
    unsigned int timeout = jiffies_32 + HZ*s ;
    while( 1 )
    {
        if( time_before_eq(jiffies_32, timeout))
        {
            __NOP ;
        }
        else 
        {
            break ;
        }
    }
}

void mdelay( uint32_t ms)
{
    unsigned int timeout = jiffies_32 + (HZ*ms)/1000;
    while( 1 )
    {
        if(time_before_eq(jiffies_32, timeout))
        {
            __NOP ;
        }
        else 
        {
            break ;
        }
    }
}

void udelay( uint32_t us)
{
    uint32_t tick_count = us * TICK_CLOCK/1000000 ;
    uint32_t tick_val ;
    int32_t tick_interval ;
    tick_val = SysTick->CTRL  ;
    tick_interval = tick_val - tick_count ;
    if( tick_interval > 0 )
    {
        while( SysTick->CTRL >  tick_interval ) ;
        return ;
    }
    tick_interval += 0xFFFFFF ; 
    while(SysTick->CTRL > tick_interval ) ;
}

void systick_handler(void)   
{   
    jiffies_32++ ;
}  

