/*
 * delay.c
 *
 *  Created on: 3 июн. 2019 г.
 *      Author: venjega
 */

#include "delay.h"


volatile uint16_t delay_count=0;


void DWT_Init(void)
{
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT_CYCCNT  = 0;
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;
}


static __inline uint32_t delta(uint32_t t0, uint32_t t1)
{
    return (t1 - t0);
}

void delay_us(uint32_t us)
{
    uint32_t t0 =  DWT->CYCCNT;
    uint32_t us_count_tic =  us * (SystemCoreClock/1000000);
    while (delta(t0,DWT->CYCCNT) < us_count_tic) ;
}

void delay_ms(uint32_t ms)
{
    uint32_t t0 =  DWT->CYCCNT;
    uint32_t us_count_tic =  ms * (SystemCoreClock/1000);
    while (delta(t0,DWT->CYCCNT) < us_count_tic) ;
}


