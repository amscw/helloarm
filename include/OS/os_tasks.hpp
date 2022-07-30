/*
 * os_tasks.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_TASKS_HPP_
#define INCLUDE_OS_OS_TASKS_HPP_

#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"


extern "C" void vTaskStateIndication( void *pvParameters );
extern "C" void vTaskChangeBri( void *pvParameters );
extern "C" void vTaskCommunication( void *pvParameters );


// -------------------------------------------------------------------------------------------------------------------

template <const uint32_t ulStackDepth, TaskFunction_t pxTaskCode> class task_c
{
protected:
  static StaticTask_t xTaskControlBlock;
  static StackType_t xStack[ulStackDepth];
  static TaskHandle_t xTask;

public:

  static void Init(const char* const pcName, void * const pvParameters, UBaseType_t uxPriority) noexcept
  {
	xTask = xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, xStack, &xTaskControlBlock);
  }

  static uint8_t PriorityGet() noexcept
  {
	  return (uint8_t)uxTaskPriorityGet(xTask);
  }

  static void PrioritySet(uint8_t newPriority) noexcept
  {
	  vTaskPrioritySet(xTask, newPriority);
  }
};

// -------------------------------------------------------------------------------------------------------------------

// регистрируем все задачи
typedef task_c<configMINIMAL_STACK_SIZE, vTaskStateIndication> taskStateIndication;
typedef task_c<configMINIMAL_STACK_SIZE, vTaskChangeBri> taskChangeBri;
typedef task_c<configMINIMAL_STACK_SIZE, vTaskCommunication> taskCommunication;



#endif /* INCLUDE_OS_OS_TASKS_HPP_ */
