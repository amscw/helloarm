/*
 * pin.hpp
 *
 *  Created on: 21 мар. 2020 г.
 *      Author: moskvin
 *      Description: cpu's pin classes
 */

#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_

#include <pin_traits.hpp>
#if defined(DEBUG_GPIO)
#include "common.hpp"
#endif
/**
 * Port
 */
template<GPIO_traits::letter_t letter> struct Port
{
	enum {
		INDEX = (static_cast<std::uint32_t>(letter) >> 15) & 0x1F,
		INDEX_MASK = 1 << INDEX,
	};

	// Enable GPIOx port clock
	static inline void ClockEnable() noexcept __attribute__((always_inline))
	{
		MDR_RST_CLK->PER_CLOCK |= INDEX_MASK;
#if defined(DEBUG_GPIO)
		PRINT("GPIO%c clock enabled", LetterChar());
#endif
	}

	// Disable GPIOx port clock
	static inline void ClockDisable() noexcept __attribute__((always_inline))
	{
		MDR_RST_CLK->PER_CLOCK &= ~INDEX_MASK;
#if defined(DEBUG_GPIO)
		PRINT("GPIO%c clock disabled", LetterChar());
#endif
	}

	static inline constexpr MDR_PORT_TypeDef volatile* GPIOx()
	{
		return reinterpret_cast<MDR_PORT_TypeDef*>(letter);
	};

#if defined(DEBUG_GPIO)
	static inline constexpr char LetterChar()
	{
		return 'A' + INDEX;
	}

	static /* inline */ void DumpPortReg(const volatile std::uint32_t &reg)  noexcept /* __attribute__((always_inline)) */
	{
		extern const char *portRegNames[];
		PRINT("GPIO%c_%s = 0x%x", LetterChar(), portRegNames[(reinterpret_cast<std::uint32_t>(&reg) >> 2) & 0xF], reg);
	}
#endif
};

/**
 * Pin
 * letter - letter of port: A, B, C, ... etc
 * n - pin number
 */
template<GPIO_traits::letter_t letter, int n> class Pin
{
	enum {
		_1BIT_FIELD_MASK = (1 << n),
		_2BIT_FIELD_MASK_INV = ~(0x3 << (n << 1)),
	};

public:
	typedef Port<letter> PortType;
	enum {NUMBER = n};


	// Configure pin as push-pull output
	static void ConfigAsOutput(GPIO_traits::func_t f, GPIO_traits::speed_t s = GPIO_traits::speed_t::MAXFAST) noexcept
	{
		std::uint32_t tempreg = 0;
		// disable pull up/down
		tempreg = PortType::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)));
		PortType::GPIOx()->PULL = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set speed
		tempreg = (PortType::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->PWR = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set func
		tempreg = (PortType::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(f) << (n << 1));
		PortType::GPIOx()->FUNC = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set as digital push-pull output
		tempreg = PortType::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		PortType::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->PD & ~_1BIT_FIELD_MASK;
		PortType::GPIOx()->PD = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->RXTX & ~_1BIT_FIELD_MASK;
		PortType::GPIOx()->RXTX = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->OE | _1BIT_FIELD_MASK;
		PortType::GPIOx()->OE = tempreg & (~JTAG_PINS(PortType::GPIOx()));
	}

	// Configure pin as open-drain output
	static void ConfigAsOutputOD(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s = GPIO_traits::speed_t::MAXFAST) noexcept
	{
		std::uint32_t tempreg = 0;
		// set speed
		tempreg = (PortType::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->PWR = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set func
		tempreg = (PortType::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(f) << (n << 1));
		PortType::GPIOx()->FUNC = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set pull-up/down
		tempreg = (PortType::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | (static_cast<std::uint32_t>(pupd) << n);
		PortType::GPIOx()->PULL = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set as digital open-drain output
		tempreg = PortType::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		PortType::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->PD | _1BIT_FIELD_MASK;
		PortType::GPIOx()->PD = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->RXTX & ~_1BIT_FIELD_MASK;
		PortType::GPIOx()->RXTX = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->OE | _1BIT_FIELD_MASK;
		PortType::GPIOx()->OE = tempreg & (~JTAG_PINS(PortType::GPIOx()));
	}

	// Configure pin as digital input
	static void ConfigAsInput(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::shm_t hyst = GPIO_traits::shm_t::_200mV, bool bFilterEn = true) noexcept
	{
		std::uint32_t tempreg = 0;
		// set func
		tempreg = (PortType::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(f) << (n << 1));
		PortType::GPIOx()->FUNC = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set pull-up/down
		tempreg = (PortType::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | (static_cast<std::uint32_t>(pupd) << n);
		PortType::GPIOx()->PULL = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set hysteresis
		tempreg = (PortType::GPIOx()->PD & (~(_1BIT_FIELD_MASK << 16))) | ((static_cast<std::uint32_t>(hyst) << n) << 16);
		PortType::GPIOx()->PD = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set filtering
		tempreg = (PortType::GPIOx()->GFEN & (~_1BIT_FIELD_MASK)) | (static_cast<std::uint32_t>(bFilterEn) << n);
		PortType::GPIOx()->GFEN = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set as digital input
		tempreg = PortType::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		PortType::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->OE & (~_1BIT_FIELD_MASK);
		PortType::GPIOx()->OE = tempreg & (~JTAG_PINS(PortType::GPIOx()));
	}

	static void ConfigAs (GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s, bool bOD, GPIO_traits::shm_t hyst, bool bFiEn) noexcept
	{
		std::uint32_t tempreg = 0;
		// set func
		tempreg = (PortType::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(f) << (n << 1));
		PortType::GPIOx()->FUNC = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set pull-up/down
		tempreg = (PortType::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | (static_cast<std::uint32_t>(pupd) << n);
		PortType::GPIOx()->PULL = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set speed
		tempreg = (PortType::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->PWR = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set open-drain
		tempreg = (PortType::GPIOx()->PD & (~_1BIT_FIELD_MASK)) | (static_cast<std::uint32_t>(bOD) << n);
		PortType::GPIOx()->PD = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set hysteresis
		tempreg = (PortType::GPIOx()->PD & (~(_1BIT_FIELD_MASK << 16))) | ((static_cast<std::uint32_t>(hyst) << n) << 16);
		PortType::GPIOx()->PD = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set filtering
		tempreg = (PortType::GPIOx()->GFEN & (~_1BIT_FIELD_MASK)) | (static_cast<std::uint32_t>(bFiEn) << n);
		PortType::GPIOx()->GFEN = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// set as digital input
		tempreg = PortType::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		PortType::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->OE & (~_1BIT_FIELD_MASK);
		PortType::GPIOx()->OE = tempreg & (~JTAG_PINS(PortType::GPIOx()));
	}

	static void ConfigAsAnalog (GPIO_traits::speed_t s) noexcept
	{
		std::uint32_t tempreg = 0;
		// set speed
		tempreg = (PortType::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->PWR = tempreg & (~JTAG_PINS(PortType::GPIOx()));
		// analog input?
		tempreg = PortType::GPIOx()->ANALOG & (~_1BIT_FIELD_MASK);
		PortType::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(PortType::GPIOx()));

		tempreg = PortType::GPIOx()->OE & (~_1BIT_FIELD_MASK);
		PortType::GPIOx()->OE = tempreg & (~JTAG_PINS(PortType::GPIOx()));
	}

	static inline void Set() noexcept __attribute__((always_inline))
	{
		PortType::GPIOx()->RXTX = (PortType::GPIOx()->RXTX | (1 << n)) & ~JTAG_PINS(PortType::GPIOx());
	}

	static inline void Reset() noexcept __attribute__((always_inline))
	{
		PortType::GPIOx()->RXTX &= ~((1 << n) | JTAG_PINS(PortType::GPIOx()));
	}

	static inline bool Get() noexcept __attribute__((always_inline))
	{
		return ((PortType::GPIOx()->RXTX & (1 << n)) > 0);
	}

	constexpr static inline std::uint32_t GetPinNumber() noexcept __attribute__((always_inline))
	{
		return n;
	}

	constexpr static inline MDR_PORT_TypeDef* GetPort() noexcept __attribute__((always_inline))
	{
		return reinterpret_cast<MDR_PORT_TypeDef*>(letter);
	}
};

#endif /* INCLUDE_PIN_HPP_ */
