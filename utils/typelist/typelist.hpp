/*
 * utils.hpp
 *
 *  Created on: 7 апр. 2020 г.
 *      Author: moskvin
 *      Include <iostream> in the top files, if need.
 */

#ifndef TYPELIST_HPP_
#define TYPELIST_HPP_

#include <typeinfo>
#include <utility>

//-----------------------------------------------------------------------------
// Организация списков типов
//-----------------------------------------------------------------------------
/**
 * Признак пустого списка
 */
namespace mlib {


	struct Void{};


/**
 * Список типов
 * (общий шаблон)
 */
template <typename ... Args> struct TypeList
{
	typedef mlib::Void Head;
	typedef mlib::Void Tail;
};

/**
 * Список типов
 * (частичная специализация для списка из, по крайней мере, одного элемента)
 */
template <typename H, typename ... T> struct TypeList<H, T...>
{
	typedef H Head;
	typedef TypeList<T...> Tail;
};

/**
 * Синоним для пустого списка
 */
typedef TypeList<> EmptyTypeList;

//-----------------------------------------------------------------------------
// Определение факта пустого списка
//-----------------------------------------------------------------------------
/**
 * Общий шаблон.
 * По-умолчанию список пуст
 */
template <typename TL> struct IsEmpty : std::true_type
{

};

/**
 * Полная специализация для пустого списка
 */
template<> struct IsEmpty<TypeList<mlib::Void, mlib::Void>> : std::true_type
{

};

/**
 * Частичная специализация для любого списка
 * IsEmpty "вызывает" сама себя, передавая себе TypeList<Args...>::Tail,
 * пока не дойдет до специализации с пустым списком
 */
template<typename ... Args> struct IsEmpty<TypeList<Args...>> : std::integral_constant<bool,
	std::is_same<typename TypeList<Args...>::Head, mlib::Void>::value && IsEmpty<typename TypeList<Args...>::Tail>::value>
{

};

//-----------------------------------------------------------------------------
// Определение факта наличия элемента в списке
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 * Нормально, не инстанцируется
 */
template <typename T, typename TL> struct Contains :
	std::false_type
{

};

/**
 * Частичная специализация
 * T = utils::Void, TL = Args...
 */
template <typename ... Args> struct Contains<Void, TypeList<Args...>> :
	std::false_type
{

};

/**
 * Частичная специализация
 * TL = TypeList<Args>
 */
template<typename T, typename ... Args> struct Contains<T, TypeList<Args...>> :
	std::integral_constant<
		bool,
		std::is_same<T, typename TypeList<Args...>::Head>::value ||
		Contains<T, typename TypeList<Args...>::Tail>::value
	>
{

};

//-----------------------------------------------------------------------------
// Вычисление длины списка
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 */
template <typename TL> struct Length : std::integral_constant<unsigned int, 0>
{

};

/**
 * Полная специализация
 * Список пуст
 */
template <> struct Length<EmptyTypeList> : std::integral_constant<unsigned int, 0>
{

};

/**
 * Частичная специализация:
 * TL = TypeList<Args...>
 */
template <typename ... Args> struct Length<TypeList<Args...>> :
	std::integral_constant<
		unsigned int,
		IsEmpty<TypeList<Args...>>::value ? 0 :
		1 + Length<typename TypeList<Args...>::Tail>::value
	>
{

};

//-----------------------------------------------------------------------------
// Получить тип по индексу
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 * Инстанцируется для чего угодно, кроме TypeList
 */
template <unsigned int Index, typename TL> struct TypeAt
{
	typedef mlib::Void Type;
};

/**
 * Частичная специализация
 * Index = 0
 * TL = TypeList<Args...>
 */
template<typename ... Args> struct TypeAt<0, TypeList<Args...>>
{
	typedef typename TypeList<Args...>::Head type;
};

/**
 * Частичная специализация
 * TL = TypeList<Args...>
 */
template<unsigned int Index, typename ... Args> struct TypeAt<Index, TypeList<Args...>>
{
	static_assert(Index < Length<TypeList<Args...>>::value, "N is too big");
	typedef typename TypeAt<Index-1, typename TypeList<Args...>::Tail>::type type;
};

//-----------------------------------------------------------------------------
// Оператор вывода в поток
//-----------------------------------------------------------------------------
#if defined(_GLIBCXX_IOSTREAM)
 Пустой список
std::ostream& operator<<(std::ostream& ostr, EmptyTypeList)
{
	ostr << "{}";
	return ostr;
}

template<typename TL>
void PrintTypeListHelper(TL, std::ostream& ostr)
{
}

template<typename T>
void PrintTypeListHead(T, std::ostream& ostr)
{
	ostr << typeid(T).name();
}

template<typename ...Args>
void PrintTypeListHead(TypeList<Args...> tl, std::ostream& ostr)
{
	ostr << tl;
}

template<typename Head, typename ...Args>
void PrintTypeListHelper(TypeList<Head, Args...>, std::ostream& ostr)
{
	PrintTypeListHead(Head(), ostr);
	if(!IsEmpty<TypeList<Args...>>::value)
	{
		ostr << ' ';
		PrintTypeListHelper<Args...>(TypeList<Args...>(), ostr);
	}
}

template<typename ...Args>
std::ostream& operator<<(std::ostream& ostr, TypeList<Args...> tl)
{
	ostr << '{';
	PrintTypeListHelper(tl, ostr);
	ostr << '}';
	return ostr;
}
#endif // _GLIBCXX_IOSTREAM

//-----------------------------------------------------------------------------
// Добавление в конец списка
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 * Нормально, не инстанцируется
 */
template<typename TOrTL2, typename TL> struct Append
{

};

/**
 * Частичная специализация
 * TL = TypeList<Args...>
 * добавляет тип в конец списка
 */
template<typename T, typename ... Args> struct Append<T, TypeList<Args...>>
{
	typedef TypeList<Args..., T> type;
};

/**
 * Частичная специализация
 * TOrTL2 = TypeList<Args1...> - добавляемый список
 * TL = TypeList<Args2...> - исходный список
 * добавляет список TypeList<Args1...> в конец исходного списка TypeList<Args2...>
 */
template<typename ...Args1, typename ...Args2>
struct Append<TypeList<Args1...>, TypeList<Args2...>>
{
    typedef TypeList<Args2..., Args1...> type;
};

/**
 * Тоже самое, только название другое
 */
template<typename T, typename TL> struct Add
{

};

template<typename T, typename ...Args> struct Add<T, TypeList<Args...>>
{
	typedef TypeList<Args..., T> type;
};

//-----------------------------------------------------------------------------
// Удаление элементов из списка
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 * Нормально, не инстанцируется
 */
template <typename T, typename TL> struct Remove
{

};

/**
 * Частичная специализация
 * TL = TypeList<Args...>
 *
 * Принцип: формируем новый список на основе исходного, исключая T
 *
 * если "голова" совпадает с T, то вызываем Remove еще раз и т.д., пока не совпадет.
 * При совпадении, к списку, состоящему из одной головы, добавляется результат
 * рекурсивной обработки хвоста, т.е. голова следующего TypeList,
 * к которой добавляется или не добавляется голова следующего TypeList и т.д.
 */
template <typename T, typename ... Args> struct Remove<T, TypeList<Args...>>
{
private:
	// сохраняем голову
	typedef typename TypeList<Args...>::Head Head;
	// далее рекурсивно вызываем Remove от хвоста
	typedef typename Remove<T, typename TypeList<Args...>::Tail>::type Removed;

public:
    typedef typename std::conditional<
		std::is_same<Head, T>::value,
		Removed,
		typename Append<Removed, TypeList<Head>>::type
	>::type type;
};

/**
 * Частичная специализация
 * TL = TypeList<TLAST>
 *
 * Последний элемент списка: Head = TLAST, Tail = TypeList<>
 * Если аргумент совпадает с Head, возвращаем пустой список, иначе
 * возвращаем TypeList<Head>
 */
template<typename T, typename Head> struct Remove<T, TypeList<Head>>
{
    typedef typename std::conditional<
    	std::is_same<Head, T>::value,
        EmptyTypeList,
        TypeList<Head>
    >::type type;
};

/**
 * Частичная специализация
 * пустой список
 */
template<typename T> struct Remove<T, EmptyTypeList>
{
    typedef EmptyTypeList type;
};

//-----------------------------------------------------------------------------
// Удаление дубликатов из списка
//-----------------------------------------------------------------------------
template<typename TL> struct RemoveDuplicates
{

};

/**
 * Частичная специализация
 * пустой список: с пустого списка мы ничего не можем удалить
 */
template<> struct RemoveDuplicates<EmptyTypeList>
{
    typedef EmptyTypeList type;
};

/**
 * Частичная специализация
 * T = TypeList<Args...>
 *
 * Принцип: удаляем такие же элементы, как и голова из хвоста
 * Рекурсивно вызываем функцию для хвоста
 * Объединяем голову с результатом
 */
template<typename ... Args> struct RemoveDuplicates<TypeList<Args...>>
{
private:
    typedef TypeList<Args...> TL;
    typedef typename Remove<typename TL::Head, typename TL::Tail>::type HeadRemovedFromTail;
    typedef typename RemoveDuplicates<HeadRemovedFromTail>::type TailWithoutDuplicates;

public:
    typedef typename Append<TailWithoutDuplicates, TypeList<typename TL::Head>>::type type;
};

} // utils

#endif /* TYPELIST_HPP_ */
