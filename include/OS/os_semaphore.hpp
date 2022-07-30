/*
 * os_semaphore.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_SEMAPHORE_HPP_
#define INCLUDE_OS_OS_SEMAPHORE_HPP_

#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t ComminicationSemaphore;

// -------------------------------------------------------------------------------------------------------------------

template<SemaphoreHandle_t* xSema> class semaphore_c
{
  static StaticSemaphore_t xSemaControlBlock;

public:

  static bool Init() noexcept
  {
	  *xSema = xSemaphoreCreateBinaryStatic(&xSemaControlBlock);
	  if (*xSema == NULL) return false;
	  else return true;
  }

  static bool Give() noexcept
  {
	  return (bool)xSemaphoreGive(*xSema);
  }

  static bool  GiveFromISR (portBASE_TYPE *pxHigherPriorityTaskWoken) noexcept
  {
	  return (bool)xSemaphoreGiveFromISR(*xSema, pxHigherPriorityTaskWoken);
  }

  static bool Take(TickType_t xTicksToWait = portMAX_DELAY) noexcept
  {
	  return (bool)xSemaphoreTake(*xSema, xTicksToWait);  // xTicksToWait = portMAX_DELAY - задача блокирована, пока не получит семафор
  }
};

// -------------------------------------------------------------------------------------------------------------------

typedef semaphore_c<&ComminicationSemaphore> communication_semphr;



#endif /* INCLUDE_OS_OS_SEMAPHORE_HPP_ */
