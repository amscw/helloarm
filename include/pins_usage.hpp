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
using PA1 = Pin<GPIO_traits::letter_t::A, 1>;	// SUS_S3#
using PA2 = Pin<GPIO_traits::letter_t::A, 2>;	// SUS_S4#
using PA3 = Pin<GPIO_traits::letter_t::A, 3>;	// SUS_S5#

// leds
using PD5 = Pin<GPIO_traits::letter_t::D, 5>;	// LED_PWR_ON
using PD6 = Pin<GPIO_traits::letter_t::D, 6>;	// LED_PWR_OFF

// screen backlight
using PE0 = Pin<GPIO_traits::letter_t::E, 0>;	// BKLT_CTRL
using PE1 = Pin<GPIO_traits::letter_t::E, 1>;	// BKLT_EN

// screen power control
using PE2 = Pin<GPIO_traits::letter_t::E, 2>;	// LVDS_VDD_EN

// i2c
using PC2 = Pin<GPIO_traits::letter_t::C, 2>;	// I2C_CONN
using PC0 = Pin<GPIO_traits::letter_t::C, 0>;	// SCL
using PC1 = Pin<GPIO_traits::letter_t::C, 1>;	// SDA

#endif /* INCLUDE_PINS_USAGE_HPP_ */
