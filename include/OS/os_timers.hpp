/*
 * os_timers.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_TIMERS_HPP_
#define INCLUDE_OS_OS_TIMERS_HPP_

#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "timers.h"


template<TimerHandle_t* xTimer> class timer_c
{
  static StaticTimer_t xTimerControlBlock;

public:

  static void init (const char * const pcTimerName, const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction) noexcept
  {
	 *xTimer = xTimerCreateStatic(pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction, &xTimerControlBlock);
  }

  static void start(TickType_t xTicksToWait = 0) noexcept
  {
  	 xTimerStart(*xTimer, xTicksToWait);
  }
};


#endif /* INCLUDE_OS_OS_TIMERS_HPP_ */
