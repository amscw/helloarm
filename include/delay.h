/*
 * delay.h
 *
 *  Created on: 3 июн. 2019 г.
 *      Author: venjega
 */

#ifndef INCLUDE_DELAY_H_
#define INCLUDE_DELAY_H_

#include "MDR32Fx.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC


void DWT_Init(void);
void delay_us(uint32_t);
void delay_ms(uint32_t);




#endif /* INCLUDE_DELAY_H_ */
