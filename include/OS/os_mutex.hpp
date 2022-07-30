/*
 * os_mutex.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_MUTEX_HPP_
#define INCLUDE_OS_OS_MUTEX_HPP_

#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"


template<SemaphoreHandle_t* xMutex> class mutex_c
{
	static StaticSemaphore_t xMutexControlBlock;

public:

	static bool init() noexcept
	{
		*xMutex = xSemaphoreCreateMutexStatic(&xMutexControlBlock);
		if (*xMutex == NULL) return false;
		else return true;
	}

	static bool lock(TickType_t xTicksToWait = portMAX_DELAY) noexcept
	{
		return (bool)xSemaphoreTake(*xMutex, xTicksToWait);
	}

	static bool unlock() noexcept
	{
		return (bool)xSemaphoreGive(*xMutex);
	}
};


#endif /* INCLUDE_OS_OS_MUTEX_HPP_ */
