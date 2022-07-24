/*
 * pin_traits.hpp
 *
 *  Created on: 12 апр. 2020 г.
 *      Author: moskvin
 *      Descriptioт: Platform's specific constants
 */

#ifndef INCLUDE_PIN_TRAITS_HPP_
#define INCLUDE_PIN_TRAITS_HPP_


#include <cstdint>
extern "C" {
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
}

namespace GPIO_traits {

enum letter_t : std::uint32_t {
	A = MDR_PORTA_BASE,
	B = MDR_PORTB_BASE,
	C = MDR_PORTC_BASE,
	D = MDR_PORTD_BASE,
	E = MDR_PORTE_BASE,
	F = MDR_PORTF_BASE
};

// port's register constants
enum class mode_t : std::uint8_t { ANALOG, DIGITAL };
enum class speed_t : std::uint8_t {	OUTPUT_OFF, SLOW, FAST, MAXFAST };
enum class func_t : std::uint8_t { PORT, MAIN, ALTER, OVERRID };
enum class pd_t : std::uint8_t { DRIVER, OPEN };
enum class pull_t : std::uint32_t { NO, UP = 1 << 16, DOWN = 1, BOTH = UP | DOWN };
enum class oe_t : std::uint8_t {INPUT, OUTPUT };
enum class shm_t : std::uint8_t {_200mV, _400mV };

} // GPIO



#endif /* INCLUDE_PIN_TRAITS_HPP_ */
