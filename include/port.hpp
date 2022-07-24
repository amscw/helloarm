/*
 * port.hpp
 *
 *  Created on: 12 апр. 2020 г.
 *      Author: moskvin
 */

#ifndef INCLUDE_PORT_HPP_
#define INCLUDE_PORT_HPP_

#include "pin.hpp"
#include "typelist.hpp"
#include "pinlist.hpp"
#include "bithacks.hpp"
#include "common.hpp"

//-----------------------------------------------------------------------------
// Выполняет конфигурацию для отдельно взятого порта
//-----------------------------------------------------------------------------
/**
 * Структура нужна для обхода ограничения частичной специализации шаблонов функций
 */
template<typename TL1, typename TL2> struct MultiplePorts;

template<typename ... Pins> struct MultiplePorts<mlib::EmptyTypeList,  mlib::TypeList<Pins...>>
{
	static inline void everyClockEnable() noexcept {}
	static inline void everyClockDisable() noexcept {}
	static void everyAsOutput(GPIO_traits::func_t f, GPIO_traits::speed_t s) noexcept {}
	static void everyAsOutputOD(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s) noexcept{}
	static void everyAsInput(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::shm_t hyst, bool bFilterEn) noexcept {}
	static void everyAs(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s, bool bOD, GPIO_traits::shm_t hyst, bool bFiEn) {}
	static void everyAsAnalog (GPIO_traits::speed_t s) noexcept {}
	static inline void everySet () noexcept {}
	static inline void everyReset () noexcept {}
	static void everySetValue (std::uint32_t value) noexcept {}
};


template<typename ... Ports, typename ... Pins> struct MultiplePorts<mlib::TypeList<Ports...>,  mlib::TypeList<Pins...>>
{
private:
	// сохраняем исходный список
	typedef typename mlib::TypeList<Pins...> PinList;
	// откусываем "голову" списка портов
	typedef typename mlib::TypeList<Ports...>::Head Port;
	// получаем список пинов для порта "головы"
	typedef typename TakePinsAtPort<Port, mlib::TypeList<Pins...>>::type PinListAtThisPort;
	// получаем маски пинов
	enum {	// !!! WARNING !!! это будет неявное расширение до std::uint64_t так что при использовании нужен каст вниз!
		_1BIT_FIELD_MASK = std::uint16_t(MakePortMask<PinListAtThisPort>::VALUE),
		_2BIT_FIELD_MASK = std::uint32_t(mlib::ExpandBits(_1BIT_FIELD_MASK)),
		_4BIT_FIELD_MASK = std::uint64_t(mlib::ExpandBits(_2BIT_FIELD_MASK)),
		_1BIT_FIELD_MASK_INV = std::uint16_t(~_1BIT_FIELD_MASK),
		_2BIT_FIELD_MASK_INV = std::uint32_t(~_2BIT_FIELD_MASK),
		_4BIT_FIELD_MASK_INV = std::uint64_t(~_4BIT_FIELD_MASK),
	};

public:
	static inline void everyClockEnable() noexcept
	{
		Port::ClockEnable();
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyClockEnable();
	}

	static inline void everyClockDisable() noexcept
	{
		Port::ClockDisable();
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyClockDisable();
	}

	// настроить на выход
	static void everyAsOutput(GPIO_traits::func_t f, GPIO_traits::speed_t s) noexcept
	{
		std::uint32_t tempreg = 0;

		const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO_traits::speed_t, PinListAtThisPort>::Merge(s);
		const std::uint32_t OFUNC_FIELDS = BitField<2, GPIO_traits::func_t, PinListAtThisPort>::Merge(f);

		// disable pull up/down
		tempreg = Port::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)));
		Port::GPIOx()->PULL = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set speed
		tempreg = (Port::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | OSPEEDR_FIELDS;
		Port::GPIOx()->PWR = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set func
		tempreg = (Port::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | OFUNC_FIELDS;
		Port::GPIOx()->FUNC = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set as digital push-pull output
		tempreg = Port::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		Port::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->PD & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->PD = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->RXTX & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->RXTX = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set oe output
		tempreg = Port::GPIOx()->OE | _1BIT_FIELD_MASK;
		Port::GPIOx()->OE = tempreg & (~JTAG_PINS(Port::GPIOx()));

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->OTYPER);
		Port::DumpPortReg(Port::GPIOx()->OSPEEDR);
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif // DEBUG_GPIO

		// рекурсивно настраиваем остальные порты из списка
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyAsOutput(f, s);
	}

	// настроить на выход с открытым стоком
	static void everyAsOutputOD(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s) noexcept
	{
		std::uint32_t tempreg = 0;

		const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO_traits::speed_t, PinListAtThisPort>::Merge(s);

		std::uint32_t PUPDR_FIELDS;
		if (pupd == GPIO_traits::pull_t::UP) {
			PUPDR_FIELDS = (BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge((GPIO_traits::pull_t)((std::uint32_t)pupd >> 16))) << 16;
		} else PUPDR_FIELDS = BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge(pupd);

		const std::uint32_t OFUNC_FIELDS = BitField<2, GPIO_traits::func_t, PinListAtThisPort>::Merge(f);

		// set speed
		tempreg = (Port::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | OSPEEDR_FIELDS;
		Port::GPIOx()->PWR = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set func
		tempreg = (Port::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | OFUNC_FIELDS;
		Port::GPIOx()->FUNC = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set pull-up/down
		tempreg = (Port::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | PUPDR_FIELDS;
		Port::GPIOx()->PULL = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set as digital open-drain output
		tempreg = Port::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		Port::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->PD | _1BIT_FIELD_MASK;
		Port::GPIOx()->PD = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->RXTX & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->RXTX = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set oe output
		tempreg = Port::GPIOx()->OE | _1BIT_FIELD_MASK;
		Port::GPIOx()->OE = tempreg & (~JTAG_PINS(Port::GPIOx()));

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->OTYPER);
		Port::DumpPortReg(Port::GPIOx()->OSPEEDR);
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif

		// рекурсивно настраиваем остальные порты из списка
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyAsOutputOD(f, pupd, s);
	}

	// Configure pin as digital input
	static void everyAsInput(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::shm_t hyst, bool bFilterEn) noexcept
	{
		std::uint32_t tempreg = 0;

		const std::uint32_t HYST_FIELDS = BitField<1, GPIO_traits::shm_t, PinListAtThisPort>::Merge(hyst);
		const std::uint32_t FILT_FIELDS = BitField<1, bool, PinListAtThisPort>::Merge(bFilterEn);

		std::uint32_t PUPDR_FIELDS;
		if (pupd == GPIO_traits::pull_t::UP) {
			PUPDR_FIELDS = (BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge((GPIO_traits::pull_t)((std::uint32_t)pupd >> 16))) << 16;
		} else PUPDR_FIELDS = BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge(pupd);

		const std::uint32_t OFUNC_FIELDS = BitField<2, GPIO_traits::func_t, PinListAtThisPort>::Merge(f);

		// set func
		tempreg = (Port::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | OFUNC_FIELDS;
		Port::GPIOx()->FUNC = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set pull-up/down
		tempreg = (Port::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | PUPDR_FIELDS;
		Port::GPIOx()->PULL = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set hysteresis
		tempreg = (Port::GPIOx()->PD & (~(_1BIT_FIELD_MASK << 16))) | (HYST_FIELDS << 16);
		Port::GPIOx()->PD = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set filtering
		tempreg = (Port::GPIOx()->GFEN & _1BIT_FIELD_MASK_INV) | FILT_FIELDS;
		Port::GPIOx()->GFEN = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set as digital input
		tempreg = Port::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		Port::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set oe input
		tempreg = Port::GPIOx()->OE & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->OE = tempreg & (~JTAG_PINS(Port::GPIOx()));

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif

		// рекурсивно настраиваем остальные порты из списка
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyAsInput(f, pupd, hyst, bFilterEn);
	}

	static void everyAs(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s, bool bOD, GPIO_traits::shm_t hyst, bool bFiEn) noexcept
	{
		std::uint32_t tempreg = 0;

		std::uint32_t PUPDR_FIELDS;

		if (pupd == GPIO_traits::pull_t::UP) {
			PUPDR_FIELDS = (BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge((GPIO_traits::pull_t)((std::uint32_t)pupd >> 16))) << 16;
		} else PUPDR_FIELDS = BitField<1, GPIO_traits::pull_t, PinListAtThisPort>::Merge(pupd);

		const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO_traits::speed_t, PinListAtThisPort>::Merge(s);
		const std::uint32_t OFUNC_FIELDS = BitField<2, GPIO_traits::func_t, PinListAtThisPort>::Merge(f);
		const std::uint32_t HYST_FIELDS = BitField<1, GPIO_traits::shm_t, PinListAtThisPort>::Merge(hyst);
		const std::uint32_t FILT_FIELDS = BitField<1, bool, PinListAtThisPort>::Merge(bFiEn);
		const std::uint32_t ISOD_FIELDS = BitField<1, bool, PinListAtThisPort>::Merge(bOD);

		// set func
		tempreg = (Port::GPIOx()->FUNC & _2BIT_FIELD_MASK_INV) | OFUNC_FIELDS;
		Port::GPIOx()->FUNC = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set pull-up/down
		tempreg = (Port::GPIOx()->PULL & (~(_1BIT_FIELD_MASK | (_1BIT_FIELD_MASK << 16)))) | PUPDR_FIELDS;
		Port::GPIOx()->PULL = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set speed
		tempreg = (Port::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | OSPEEDR_FIELDS;
		Port::GPIOx()->PWR = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set open-drain
		tempreg = (Port::GPIOx()->PD & (~_1BIT_FIELD_MASK)) | ISOD_FIELDS;
		Port::GPIOx()->PD = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set hysteresis
		tempreg = (Port::GPIOx()->PD & (~(_1BIT_FIELD_MASK << 16))) | (HYST_FIELDS << 16);
		Port::GPIOx()->PD = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set filtering
		tempreg = (Port::GPIOx()->GFEN & _1BIT_FIELD_MASK_INV) | FILT_FIELDS;
		Port::GPIOx()->GFEN = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// set as digital input
		tempreg = Port::GPIOx()->ANALOG | _1BIT_FIELD_MASK;
		Port::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->OE & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->OE = tempreg & (~JTAG_PINS(Port::GPIOx()));

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->OTYPER);
		Port::DumpPortReg(Port::GPIOx()->OSPEEDR);
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->AFR[0]);
		Port::DumpPortReg(Port::GPIOx()->AFR[1]);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif

		// рекурсивно настраиваем остальные порты из списка
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyAs(f, pupd, s, bOD, hyst, bFiEn);
	}

	static void everyAsAnalog(GPIO_traits::speed_t s) noexcept
	{
		std::uint32_t tempreg = 0;

		const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO_traits::speed_t, PinListAtThisPort>::Merge(s);

		// set speed
		tempreg = (Port::GPIOx()->PWR & _2BIT_FIELD_MASK_INV) | OSPEEDR_FIELDS;
		Port::GPIOx()->PWR = tempreg & (~JTAG_PINS(Port::GPIOx()));
		// analog input?
		tempreg = Port::GPIOx()->ANALOG & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->ANALOG = tempreg & (~JTAG_PINS(Port::GPIOx()));

		tempreg = Port::GPIOx()->OE & _1BIT_FIELD_MASK_INV;
		Port::GPIOx()->OE = tempreg & (~JTAG_PINS(Port::GPIOx()));

		// рекурсивно настраиваем остальные порты из списка
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyAsAnalog(s);
	}

	static inline void everySet () noexcept
	{
		Port::GPIOx()->RXTX = (Port::GPIOx()->RXTX | _1BIT_FIELD_MASK) & ~JTAG_PINS(Port::GPIOx());
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everySet();
	}

	static void everySetValue (std::uint32_t value) noexcept
	{
		std::uint32_t tempreg = 0;

		const std::uint32_t RESULT = PinMapper<PinListAtThisPort>::MapToPins(value);

		tempreg = (Port::GPIOx()->RXTX & _1BIT_FIELD_MASK_INV) | RESULT;
		Port::GPIOx()->RXTX = tempreg & (~JTAG_PINS(Port::GPIOx()));

		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everySetValue(value);
	}

	static inline void everyReset () noexcept
	{
		Port::GPIOx()->RXTX &= ~(_1BIT_FIELD_MASK | JTAG_PINS(Port::GPIOx()));
		MultiplePorts<typename mlib::TypeList<Ports...>::Tail, PinList>::everyReset();
	}
};



//-----------------------------------------------------------------------------
// Выполняет конфигурацию произвольного списка пинов
//-----------------------------------------------------------------------------
template<typename TL> struct MultiplePins;

/**
 * Частичная спициализация
 * TL = TypeList<PWs...>
 */
template<typename ... PWs> class MultiplePins<mlib::TypeList<PWs...>>
{
	// сохраняем список пинов
	typedef mlib::TypeList<PWs...> PinList;
	// сохраняем список их портов, удаляем дубликаты
	typedef typename mlib::RemoveDuplicates<typename MakePorts<PinList>::type>::type PortList;

public:
	static inline void ClockEnable() noexcept
	{
		MultiplePorts<PortList, PinList>::everyClockEnable();
	}

	static inline void ClockDisable() noexcept
	{
		MultiplePorts<PortList, PinList>::everyClockDisable();
	}

	static inline void ConfigAsOutput(GPIO_traits::func_t f, GPIO_traits::speed_t s = GPIO_traits::speed_t::MAXFAST) noexcept
	{
		MultiplePorts<PortList, PinList>::everyAsOutput(f, s);
	}

	static inline void ConfigAsOutputOD(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s = GPIO_traits::speed_t::MAXFAST) noexcept
	{
		MultiplePorts<PortList, PinList>::everyAsOutputOD(f,pupd, s);
	}

	static inline void ConfigAsInput(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::shm_t hyst, bool bFilt) noexcept
	{
		MultiplePorts<PortList, PinList>::everyAsInput(f, pupd, hyst, bFilt);
	}

	static inline void ConfigAs(GPIO_traits::func_t f, GPIO_traits::pull_t pupd, GPIO_traits::speed_t s, bool bOD, GPIO_traits::shm_t hyst, bool bFiEn) noexcept
	{
		MultiplePorts<PortList, PinList>::everyAs(f, pupd, s, bOD, hyst, bFiEn);
	}

	static inline void ConfigAsAnalog(GPIO_traits::speed_t s =  GPIO_traits::speed_t::MAXFAST) noexcept
	{
		MultiplePorts<PortList, PinList>::everyAsAnalog(s);
	}

	static inline void Set () noexcept
	{
		MultiplePorts<PortList, PinList>::everySet();
	}

	static inline void SetValue (std::uint32_t value) noexcept
	{
		MultiplePorts<PortList, PinList>::everySetValue(value);
	}

	static inline void Reset () noexcept
	{
		MultiplePorts<PortList, PinList>::everyReset();
	}
};

#endif /* INCLUDE_PORT_HPP_ */
