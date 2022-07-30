/*
 * os_queue.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_QUEUE_HPP_
#define INCLUDE_OS_OS_QUEUE_HPP_

#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "queue.h"


extern QueueHandle_t ChangeBriQueue;

// -----------------------------------------------------------------------------------------------------------


template<QueueHandle_t* xHandle, class T, size_t size> class queue_c
{
  static StaticQueue_t  xQueueDefinition;
  static T  xStorage[size];

public:

  static bool Init() noexcept
  {
	  *xHandle = xQueueCreateStatic(size, sizeof(T), reinterpret_cast<uint8_t*>(xStorage), &xQueueDefinition);
	  if (*xHandle == NULL) return false;
	  else return true;
  }

  static bool Receive(T * val, TickType_t xTicksToWait = portMAX_DELAY) noexcept
  {
	  return (bool)xQueueReceive(*xHandle, val, xTicksToWait);
  }

  static bool Send(const T* val, TickType_t xTicksToWait = portMAX_DELAY) noexcept
  {
	  return (bool)xQueueSendToBack(*xHandle, val, xTicksToWait);
  }

  static bool ReceiveFromISR(T * val, BaseType_t * const pxHigherPriorityTaskWoken) noexcept
  {
	  return (bool)xQueueReceiveFromISR(*xHandle, val, pxHigherPriorityTaskWoken);
  }

  static bool SendFromISR(const T* val, BaseType_t * const pxHigherPriorityTaskWoken) noexcept
  {
	  return (bool)xQueueSendToBackFromISR(*xHandle, val, pxHigherPriorityTaskWoken);
  }

};

// ----------------------------------------------------------------------------------------------------------

typedef queue_c <&ChangeBriQueue, uint8_t, 1> changeBri_queue;


#endif /* INCLUDE_OS_OS_QUEUE_HPP_ */
