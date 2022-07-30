/*
 * os.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: venjega
 */

#ifndef INCLUDE_OS_OS_HPP_
#define INCLUDE_OS_OS_HPP_

extern "C" {
#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "croutine.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "delay.h"
}
#include "pins_usage.hpp"


extern"C" void OS_Start();



#endif /* INCLUDE_OS_OS_HPP_ */
