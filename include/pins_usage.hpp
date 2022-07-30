/*
 * pins_usage.hpp
 *
 *  Created on: 26 мар. 2020 г.
 *      Author: moskw
 */

#ifndef INCLUDE_PINS_USAGE_HPP_
#define INCLUDE_PINS_USAGE_HPP_

#include "port.hpp"

/**
 * ALl application pins MUST be defined here!
 */


using PB0 = Pin<GPIO_traits::letter_t::B, 0>;	// COMe_SMB_ALERT_S5#  ???


// inputs
using PA0 = Pin<GPIO_traits::letter_t::A, 0>;	// EN_S5			// пока не используется
using PA1 = Pin<GPIO_traits::letter_t::A, 1>;	// SUS_S3#
using PA2 = Pin<GPIO_traits::letter_t::A, 2>;	// SUS_S4#
using PA3 = Pin<GPIO_traits::letter_t::A, 3>;	// SUS_S5#
using PA4 = Pin<GPIO_traits::letter_t::A, 4>;	// PWRBTN#			// пока не используется
using PA6 = Pin<GPIO_traits::letter_t::A, 6>;	// SUS_STAT#		// пока не используется
using PA7 = Pin<GPIO_traits::letter_t::A, 7>;	// COMe_WAKE1#		// пока не используется
using PE3 = Pin<GPIO_traits::letter_t::E, 3>;	// WDT				// пока не используется
using PE6 = Pin<GPIO_traits::letter_t::E, 6>;	// WAKE#			// пока не используется
using PE7 = Pin<GPIO_traits::letter_t::E, 7>;	// COMe_BATLOW#		// пока не используется

// outputs
using PA5 = Pin<GPIO_traits::letter_t::A, 5>;	// ATX_PSON#		// пока не используется
using PD7 = Pin<GPIO_traits::letter_t::D, 7>;	// 12V_ext_EN		// пока не используется
using PE2 = Pin<GPIO_traits::letter_t::E, 2>;	// LVDS_VDD_EN

// leds
using PD5 = Pin<GPIO_traits::letter_t::D, 5>;	// LED_PWR_ON
using PD6 = Pin<GPIO_traits::letter_t::D, 6>;	// LED_PWR_OFF

// screen backlight
using PE0 = Pin<GPIO_traits::letter_t::E, 0>;	// BKLT_CTRL
using PE1 = Pin<GPIO_traits::letter_t::E, 1>;	// BKLT_EN

// DDC / EDID EEPROM
using PB1 = Pin<GPIO_traits::letter_t::B, 1>;	// EDIDEEP_PROG		// пока не используется
using PB4 = Pin<GPIO_traits::letter_t::B, 4>;	// EDIDEEP2_PROG	// пока не используется

using PB2 = Pin<GPIO_traits::letter_t::B, 2>;	// DDC_SINK_SEL		// пока не используется

using PB3 = Pin<GPIO_traits::letter_t::B, 3>;	// HPD				// пока не используется

// uart
using PB5 = Pin<GPIO_traits::letter_t::B, 5>;	// TX				// пока не используется
using PB6 = Pin<GPIO_traits::letter_t::B, 6>;	// RX				// пока не используется

// i2c
using PC2 = Pin<GPIO_traits::letter_t::C, 2>;	// I2C_CONN
using PC0 = Pin<GPIO_traits::letter_t::C, 0>;	// SCL
using PC1 = Pin<GPIO_traits::letter_t::C, 1>;	// SDA




#endif /* INCLUDE_PINS_USAGE_HPP_ */
