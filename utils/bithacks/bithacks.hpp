/*
 * bithacks.hpp
 *
 *  Created on: 11 апр. 2020 г.
 *      Author: moskvin
 */

#ifndef BITHACKS_HPP_
#define BITHACKS_HPP_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace mlib {
//-----------------------------------------------------------------------------
// развернуть биты (отзеркалить)
//-----------------------------------------------------------------------------
template<typename T> constexpr T MirrorBits(T value, int bits);

template<> constexpr std::uint32_t MirrorBits<std::uint32_t>(std::uint32_t x, int bits)
{
    x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
    x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
    x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
    return x >> (32 - bits);
}

template<> constexpr std::uint16_t MirrorBits<std::uint16_t>(std::uint16_t x, int bits)
{
    x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
    x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
    x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
    x = ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8);
    return x >> (16 - bits);
}

template<> constexpr std::uint8_t MirrorBits<std::uint8_t>(std::uint8_t x, int bits)
{
    x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
    x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
    x = ((x & 0x0F) << 4) | ((x & 0xF0) >> 4);
    return x >> (8 - bits);
}

//-----------------------------------------------------------------------------
// растянуть биты (расширение разрядности)
//-----------------------------------------------------------------------------
template<typename T> struct return_item
{
	typedef T type;
};

template<> struct return_item<std::uint8_t>
{
	typedef std::uint16_t type;
};

template<> struct return_item<std::uint16_t>
{
	typedef std::uint32_t type;
};

template<> struct return_item<std::uint32_t>
{
	typedef std::uint64_t type;
};

template<typename T> constexpr typename return_item<T>::type ExpandBits(T x);

/**
 * 16 бит в 32
 * @param x
 * @return
 */
template<> constexpr typename return_item<std::uint16_t>::type ExpandBits<std::uint16_t>(std::uint16_t x)
{
	std::uint32_t y(x);
    y = (y | (y << 8)) & 0x00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F;
    y = (y | (y << 2)) & 0x33333333;
    y = (y | (y << 1)) & 0x55555555;
    return y | (y << 1);
}

/**
 * 32 бит в 64
 * @param x
 * @return
 */
template<> constexpr typename return_item<std::uint32_t>::type ExpandBits<std::uint32_t>(std::uint32_t x)
{
	std::uint64_t y(x);
	y = (y | (y << 16)) & 0x0000FFFF0000FFFF;
    y = (y | (y << 8)) & 0x00FF00FF00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F0F0F0F0F;
    y = (y | (y << 2)) & 0x3333333333333333;
    y = (y | (y << 1)) & 0x5555555555555555;
    return y | (y << 1);
}

//-----------------------------------------------------------------------------
// антидребезг битов
//-----------------------------------------------------------------------------
/**
 * На вход три раза подряд должна получить одинаковый сэмпл, чтобы вернула установившееся значение.
 * Если хотя бы один в серии из трех отличается, надо поновой три раза проход.
 * Поэтому условием выхода из цикла должно быть совпадение сэмпла с возвращенным значением, либо таймаут.
 * https://embedders.org/blog/gdi/debouncing.html
 *
 * @param sample
 * @return
 */
template<typename T> T DebounceBits(T sample)
{
	// c нецелочисленным параметром шаблона инстанс не скомпилируется (правило DCL50-CPP)
	typedef typename std::enable_if<std::is_integral<T>::value, T>::type item_type;

    static item_type state, cnt0, cnt1;
    item_type delta, toggle;

    // отражает изменение соответствующего разряда
    delta = sample ^ state;

    // переключает биты 1-ого разряда вертикального счетчика
    cnt1 = cnt1 ^ cnt0;
    // переключает биты 0-ого разряда вертикального счетчика
    cnt0 = ~cnt0;

    // сбросить счетчик, если не досчитал до трех (не достиг установившегося значения)
    cnt0 &= delta;
    cnt1 &= delta;

    // переключает разряды, для которых счетчик досчитал до трех
    toggle = cnt0 & cnt1;
    state ^= toggle;

    return state;
}

/**
 * Тоже самое, только в виде функтора.
 * Функторы позволяют использовать в приложении несколько дебаунсеров,
 * так как каждый из них хранит свое собственное состояние
 */
template<typename T> struct Debouncer
{
	// c нецелочисленным параметром шаблона инстанс не скомпилируется (правило DCL50-CPP)
	typedef typename std::enable_if<std::is_integral<T>::value, T>::type item_type;

	item_type operator()(T sample) noexcept
	{
	    item_type delta, toggle;
	    // отражает изменение соответствующего разряда
	    delta = sample ^ state;

	    // переключает биты 1-ого разряда вертикального счетчика
	    cnt1 = cnt1 ^ cnt0;
	    // переключает биты 0-ого разряда вертикального счетчика
	    cnt0 = ~cnt0;

	    // сбросить счетчик, если не досчитал до трех (не достиг установившегося значения)
	    cnt0 &= delta;
	    cnt1 &= delta;

	    // переключает разряды, для которых счетчик досчитал до трех
	    toggle = cnt0 & cnt1;
	    state ^= toggle;

	    return state;
	}

private:
	item_type state, cnt0, cnt1;
};

//-----------------------------------------------------------------------------
// изменить byte-order
//-----------------------------------------------------------------------------
template<typename T> inline T SwapBytes(T value);

template<> /* constexpr */ inline std::uint16_t SwapBytes<std::uint16_t>(std::uint16_t value)
{
	union {
		struct {
			std::uint8_t L;
			std::uint8_t H;
		} __attribute__((packed)) bytes;
		std::uint16_t word;
	} tmp = { .word = value };

	std::swap(tmp.bytes.L, tmp.bytes.H);

	return tmp.word;
}

template<> /* constexpr */ inline std::uint32_t SwapBytes<std::uint32_t>(std::uint32_t value)
{
	union word_t {
		struct {
			std::uint8_t lbyte;
			std::uint8_t hbyte;
		} __attribute__((packed)) bytes;
		std::uint16_t value;
	};

	union {
		struct {
			union word_t lword;
			union word_t hword;
		} words;
		std::uint32_t value;
	} tmp = { .value = value };

	std::swap(tmp.words.hword.bytes.hbyte, tmp.words.hword.bytes.lbyte);
	std::swap(tmp.words.lword.bytes.hbyte, tmp.words.lword.bytes.lbyte);
	std::swap(tmp.words.hword.value, tmp.words.lword.value);

	return tmp.value;
}

}
#endif /* BITHACKS_HPP_ */
