﻿// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Licensed under the MIT License;
// you may not use this file except in compliance with the License.
//
//
// MIT License
//
// Copyright (c) 2019
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#include <stdint.h>
#include <typeindex>

#if _MSC_VER == 1900
	#include <math.h>		// @VS14: ldexpf()
	#include <new>			// @VS14: placement new with non-default ctor
#endif

#if defined _DEBUG && _MSC_VER
	#include <intrin.h>		// __debugbreak()
#endif

#ifdef __GNUC__
	#include <math.h>		// @GCC: ldexpf()
	#include <utility>		// @GCC: std::conditional<>, move(), forward()
#endif

#define HFSM_INLINE														  //inline

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

#if INTPTR_MAX == INT64_MAX
	#define HFSM_64BIT_OR_32BIT(p64, p32)		p64
#else
	#define HFSM_64BIT_OR_32BIT(p64, p32)		p32
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG && defined _MSC_VER
	#define HFSM_BREAK()						__debugbreak()
#else
	#define HFSM_BREAK()						((void) 0)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef _DEBUG
	#define HFSM_IF_DEBUG(...)					__VA_ARGS__
	#define HFSM_UNLESS_DEBUG(...)
	#define HFSM_DEBUG_OR(y, n)					y
#else
	#define HFSM_IF_DEBUG(...)
	#define HFSM_UNLESS_DEBUG(...)				__VA_ARGS__
	#define HFSM_DEBUG_OR(y, n)					n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_ASSERT
	#define HFSM_IF_ASSERT(...)					__VA_ARGS__
	#define HFSM_CHECKED(x)						(!!(x) || (HFSM_BREAK(), 0))
	#define HFSM_ASSERT(x)						(!!(x) || (HFSM_BREAK(), 0))
	#define HFSM_ASSERT_OR(y, n)				y
#else
	#define HFSM_IF_ASSERT(...)
	#define HFSM_CHECKED(x)						x
	#define HFSM_ASSERT(x)						((void) 0)
	#define HFSM_ASSERT_OR(y, n)				n
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM_IF_LOGGER(...)										  __VA_ARGS__

	#define HFSM_LOGGER_OR(Y, N)												Y

	#define HFSM_LOG_TRANSITION(ORIGIN, TRANSITION, DESTINATION)				\
		if (_logger)															\
			_logger->recordTransition(ORIGIN, TRANSITION, DESTINATION)

	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)						\
		if (_logger)															\
			_logger->recordTaskStatus(REGION, ORIGIN, STATUS)

	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)								\
		if (_logger)															\
			_logger->recordPlanStatus(REGION, STATUS)

	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)									\
		if (_logger)															\
			_logger->recordCancelledPending(ORIGIN)

	#define HFSM_LOG_UTILITY_RESOLUTION(HEAD, PRONG, UTILITY)					\
		if (auto* const logger = control.logger())								\
			logger->recordUtilityResolution(HEAD, PRONG, UTILITY)

	#define HFSM_LOG_RANDOM_RESOLUTION(HEAD, PRONG, UTILITY)					\
		if (auto* const logger = control.logger())								\
			logger->recordRandomResolution(HEAD, PRONG, UTILITY)

#else

	#define HFSM_IF_LOGGER(...)
	#define HFSM_LOGGER_OR(Y, N)												N
	#define HFSM_LOG_TRANSITION(ORIGIN, TRANSITION, DESTINATION)
	#define HFSM_LOG_TASK_STATUS(REGION, ORIGIN, STATUS)
	#define HFSM_LOG_PLAN_STATUS(REGION, STATUS)
	#define HFSM_LOG_CANCELLED_PENDING(ORIGIN)
	#define HFSM_LOG_UTILITY_RESOLUTION(HEAD, PRONG, UTILITY)
	#define HFSM_LOG_RANDOM_RESOLUTION(HEAD, PRONG, UTILITY)

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined HFSM_ENABLE_VERBOSE_DEBUG_LOG

	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			logger->recordMethod(STATE_ID, ID)

#elif defined HFSM_ENABLE_LOG_INTERFACE

	#define HFSM_LOG_STATE_METHOD(METHOD, ID)									\
		if (auto* const logger = control.logger())								\
			log<decltype(METHOD), ID>(*logger)

#else
	#define HFSM_LOG_STATE_METHOD(METHOD, ID)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	#define HFSM_IF_STRUCTURE(...)									  __VA_ARGS__
#else
	#define HFSM_IF_STRUCTURE(...)
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _MSC_VER || defined __clang_major__ && __clang_major__ >= 7
	#define HFSM_EXPLICIT_MEMBER_SPECIALIZATION									1
#else
	#define HFSM_EXPLICIT_MEMBER_SPECIALIZATION									0
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace hfsm2 {

//------------------------------------------------------------------------------

using ShortIndex = uint8_t;
static constexpr ShortIndex	INVALID_SHORT_INDEX = UINT8_MAX;

using RegionID	 = ShortIndex;
static constexpr RegionID	INVALID_REGION_ID	= INVALID_SHORT_INDEX;

using ForkID	 = int8_t;
static constexpr ForkID		INVALID_FORK_ID		= INT8_MIN;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using LongIndex  = uint16_t;
static constexpr LongIndex	INVALID_LONG_INDEX	= UINT16_MAX;

using StateID	 = LongIndex;
static constexpr StateID	INVALID_STATE_ID	= INVALID_LONG_INDEX;

//------------------------------------------------------------------------------

namespace detail {

HFSM_IF_DEBUG(struct None {});

////////////////////////////////////////////////////////////////////////////////

template <typename T>
HFSM_INLINE
void
fill(T& a, const char value) {
	memset(&a, (int) value, sizeof(a));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, unsigned NCount>
constexpr
unsigned
count(const T(&)[NCount]) {
	return NCount;
}

//------------------------------------------------------------------------------

template <typename T, unsigned NCapacity>
HFSM_INLINE
const T*
end(const T(& a)[NCapacity]) {
	return &a[NCapacity];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TReturn, typename T, unsigned NCapacity>
HFSM_INLINE
const TReturn*
end(const T(& a)[NCapacity]) {
	return reinterpret_cast<const TReturn*>(&a[NCapacity]);
}

////////////////////////////////////////////////////////////////////////////////

template <int N1, int N2>
struct Min {
	static constexpr auto VALUE = N1 < N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <int N1, int N2>
struct Max {
	static constexpr auto VALUE = N1 > N2 ? N1 : N2;
};

//------------------------------------------------------------------------------

template <unsigned NCapacity>
struct UnsignedIndex {
	static constexpr LongIndex CAPACITY = NCapacity;

	using Type = typename std::conditional<CAPACITY <= UINT8_MAX,  uint8_t,
				 typename std::conditional<CAPACITY <= UINT16_MAX, uint16_t,
				 typename std::conditional<CAPACITY <= UINT32_MAX, uint32_t,
																   uint64_t>::type>::type>::type;

	static_assert(CAPACITY <= UINT64_MAX, "STATIC ASSERT");
};

////////////////////////////////////////////////////////////////////////////////

template <int>
struct StaticPrintConst;

template <typename>
struct StaticPrintType;

//------------------------------------------------------------------------------

template <unsigned V1, unsigned V2>
struct StaticAssertEquality;

template <unsigned V1>
struct StaticAssertEquality<V1, V1> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
class Iterator {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	HFSM_INLINE Iterator(Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE		  Item& operator *()	   { return  _container[_cursor]; }
	HFSM_INLINE const Item& operator *() const { return  _container[_cursor]; }

	HFSM_INLINE		  Item* operator->()	   { return &_container[_cursor]; }
	HFSM_INLINE const Item* operator->() const { return &_container[_cursor]; }

private:
	Container& _container;

	LongIndex _cursor;
};

//------------------------------------------------------------------------------

template <typename TContainer>
class Iterator<const TContainer> {
public:
	using Container = TContainer;
	using Item = typename Container::Item;

	template <typename T, LongIndex NCapacity>
	friend class Array;

private:
	HFSM_INLINE Iterator(const Container& container, const LongIndex cursor)
		: _container(container)
		, _cursor(cursor)
	{}

public:
	HFSM_INLINE bool operator != (const Iterator<const Container>& dummy) const;

	HFSM_INLINE Iterator& operator ++();

	HFSM_INLINE const Item& operator *() const { return _container[_cursor]; }

	HFSM_INLINE const Item* operator->() const { return &operator *();		 }

private:
	const Container& _container;

	LongIndex _cursor;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
bool
Iterator<TContainer>::operator != (const Iterator<TContainer>& HFSM_IF_ASSERT(dummy)) const {
	HFSM_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename TContainer>
Iterator<TContainer>&
Iterator<TContainer>::operator ++() {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TContainer>
bool
Iterator<const TContainer>::operator != (const Iterator<const TContainer>& HFSM_IF_ASSERT(dummy)) const {
	HFSM_ASSERT(&_container == &dummy._container);

	return _cursor != _container.limit();
}

//------------------------------------------------------------------------------

template <typename TContainer>
Iterator<const TContainer>&
Iterator<const TContainer>::operator ++() {
	_cursor = _container.next(_cursor);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NCapacity>
class StaticArray {
public:
	static constexpr LongIndex CAPACITY = NCapacity;
	static constexpr LongIndex DUMMY	= INVALID_LONG_INDEX;

	using Item  = T;
	using Index = typename UnsignedIndex<CAPACITY>::Type;

public:
	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item filler);

	HFSM_INLINE		  Item& operator[] (const LongIndex i);
	HFSM_INLINE const Item& operator[] (const LongIndex i) const;

	HFSM_INLINE LongIndex count() const						{ return CAPACITY;									}

	HFSM_INLINE void fill(const Item filler);

	HFSM_INLINE Iterator<	   StaticArray>  begin()		{ return Iterator<		StaticArray>(*this, 0);		}
	HFSM_INLINE Iterator<const StaticArray>  begin() const	{ return Iterator<const StaticArray>(*this, 0);		}
	HFSM_INLINE Iterator<const StaticArray> cbegin() const	{ return Iterator<const StaticArray>(*this, 0);		}

	HFSM_INLINE Iterator<	   StaticArray>	 end()			{ return Iterator<		StaticArray>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const StaticArray>	 end() const	{ return Iterator<const StaticArray>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const StaticArray> cend() const	{ return Iterator<const StaticArray>(*this, DUMMY);	}

private:
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
struct StaticArray<T, 0> {
	using Item  = T;

	HFSM_INLINE StaticArray() = default;
	HFSM_INLINE StaticArray(const Item)											{}
};

//------------------------------------------------------------------------------

template <typename T, LongIndex NCapacity>
class Array {
	template <typename>
	friend class Iterator;

public:
	static constexpr LongIndex CAPACITY = NCapacity;
	static constexpr LongIndex DUMMY	= INVALID_LONG_INDEX;

	using Item = T;

public:
	HFSM_INLINE void clear()														{ _count = 0;		}

	template <typename TValue>
	HFSM_INLINE LongIndex operator << (TValue&& value);

	HFSM_INLINE		  Item& operator[] (const LongIndex i);
	HFSM_INLINE const Item& operator[] (const LongIndex i) const;

	HFSM_INLINE LongIndex count() const												{ return _count;	}

	HFSM_INLINE Iterator<	   Array>  begin()			{ return Iterator<		Array>(*this, 0);		}
	HFSM_INLINE Iterator<const Array>  begin() const	{ return Iterator<const Array>(*this, 0);		}
	HFSM_INLINE Iterator<const Array> cbegin() const	{ return Iterator<const Array>(*this, 0);		}

	HFSM_INLINE Iterator<	   Array>	 end()			{ return Iterator<		Array>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const Array>	 end() const	{ return Iterator<const Array>(*this, DUMMY);	}
	HFSM_INLINE Iterator<const Array>   cend() const	{ return Iterator<const Array>(*this, DUMMY);	}

private:
	HFSM_INLINE LongIndex next(const LongIndex i) const								{ return i + 1;		}
	HFSM_INLINE LongIndex limit() const												{ return _count;	}

private:
	LongIndex _count = 0;
	Item _items[CAPACITY];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
class Array<T, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
StaticArray<T, NC>::StaticArray(const Item filler) {
	fill(filler);
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T&
StaticArray<T, NC>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
StaticArray<T, NC>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
void
StaticArray<T, NC>::fill(const Item filler) {
	for (LongIndex i = 0; i < CAPACITY; ++i)
		_items[i] = filler;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, LongIndex NC>
template <typename TValue>
LongIndex
Array<T, NC>::operator << (TValue&& value) {
	HFSM_ASSERT(_count < CAPACITY);

	new (&_items[_count]) Item{std::move(value)};

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T, LongIndex NC>
T&
Array<T, NC>::operator[] (const LongIndex i) {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, LongIndex NC>
const T&
Array<T, NC>::operator[] (const LongIndex i) const {
	HFSM_ASSERT(0 <= i && i < CAPACITY);

	return _items[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct Units {
	ShortIndex unit;
	ShortIndex width;
};

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
class BitArray final {
public:
	using Index	= TIndex;
	using Unit	= unsigned char;

	static constexpr Index CAPACITY	= NCapacity;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class Bits {
		template <typename, ShortIndex>
		friend class BitArray;

	private:
		HFSM_INLINE explicit Bits(Unit* const storage,
								  const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void clear();

		template <ShortIndex NIndex>
		HFSM_INLINE bool get() const;

		template <ShortIndex NIndex>
		HFSM_INLINE void set();

		template <ShortIndex NIndex>
		HFSM_INLINE void reset();

		HFSM_INLINE bool get  (const Index index) const;
		HFSM_INLINE void set  (const Index index);
		HFSM_INLINE void reset(const Index index);

	private:
		Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	class ConstBits {
		template <typename, ShortIndex>
		friend class BitArray;

	private:
		HFSM_INLINE explicit ConstBits(const Unit* const storage,
									   const Index width)
			: _storage{storage}
			, _width{width}
		{}

	public:
		HFSM_INLINE explicit operator bool() const;

		template <ShortIndex NIndex>
		HFSM_INLINE bool get() const;

		HFSM_INLINE bool get(const Index index) const;

	private:
		const Unit* const _storage;
		const Index _width;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	BitArray() {
		clear();
	}

	HFSM_INLINE void clear();

	template <ShortIndex NIndex>
	HFSM_INLINE bool get() const;

	template <ShortIndex NIndex>
	HFSM_INLINE void set();

	template <ShortIndex NIndex>
	HFSM_INLINE void reset();

	HFSM_INLINE bool get  (const Index index) const;
	HFSM_INLINE void set  (const Index index);
	HFSM_INLINE void reset(const Index index);

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM_INLINE		 Bits bits();

	template <ShortIndex NUnit, ShortIndex NWidth>
	HFSM_INLINE ConstBits bits() const;


	HFSM_INLINE		 Bits bits(const Units& units);
	HFSM_INLINE ConstBits bits(const Units& units) const;

private:
	Unit _storage[CAPACITY];
};

//------------------------------------------------------------------------------

template <typename TIndex>
class BitArray<TIndex, 0> final {
public:
	HFSM_INLINE void clear() {}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
BitArray<TIndex, NCapacity>::Bits::operator bool() const {
	const ShortIndex fullUnits = _width / (sizeof(Unit) * 8);

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const ShortIndex bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::clear() {
	const Index count = (_width + 7) / (sizeof(Unit) * 8);

	for (Index i = 0; i < count; ++i)
		_storage[i] = Unit{0};
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::Bits::get() const {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::Bits::set() {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::Bits::reset() {
	constexpr Index INDEX = NIndex;
	HFSM_ASSERT(INDEX < _width);

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::Bits::get(const Index index) const {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::set(const Index index) {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::Bits::reset(const Index index) {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
BitArray<TIndex, NCapacity>::ConstBits::operator bool() const {
	const ShortIndex fullUnits = _width / (sizeof(Unit) * 8);

	for (Index i = 0; i < fullUnits; ++i)
		if (_storage[i])
			return true;

	const ShortIndex bit = _width % (sizeof(Unit) * 8);
	const Unit mask = (1 << bit) - 1;
	const Unit& u = _storage[fullUnits];

	return (u & mask) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::ConstBits::get() const {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < _width, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::ConstBits::get(const Index index) const {
	HFSM_ASSERT(index < _width);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::clear() {
	for (Unit& unit: _storage)
		unit = Unit{0};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
bool
BitArray<TIndex, NCapacity>::get() const {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	return (_storage[BIT_UNIT] & MASK) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::set() {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] |= MASK;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NIndex>
void
BitArray<TIndex, NCapacity>::reset() {
	constexpr Index INDEX = NIndex;
	static_assert(INDEX < CAPACITY, "");

	constexpr Index BIT_UNIT  = INDEX / (sizeof(Unit) * 8);
	constexpr Index BIT_INDEX = INDEX % (sizeof(Unit) * 8);
	constexpr Unit MASK = 1 << BIT_INDEX;

	_storage[BIT_UNIT] &= ~MASK;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
bool
BitArray<TIndex, NCapacity>::get(const Index index) const {
	HFSM_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	return (_storage[unit] & mask) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::set(const Index index) {
	HFSM_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] |= mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
void
BitArray<TIndex, NCapacity>::reset(const Index index) {
	HFSM_ASSERT(index < CAPACITY);

	const Index unit = index / (sizeof(Unit) * 8);
	const Index bit  = index % (sizeof(Unit) * 8);
	const Unit mask = 1 << bit;

	_storage[unit] &= ~mask;
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NUnit, ShortIndex NWidth>
typename BitArray<TIndex, NCapacity>::Bits
BitArray<TIndex, NCapacity>::bits() {
	constexpr ShortIndex UNIT  = NUnit;
	constexpr ShortIndex WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return Bits{_storage + UNIT, WIDTH};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
template <ShortIndex NUnit, ShortIndex NWidth>
typename BitArray<TIndex, NCapacity>::ConstBits
BitArray<TIndex, NCapacity>::bits() const {
	constexpr ShortIndex UNIT  = NUnit;
	constexpr ShortIndex WIDTH = NWidth;
	static_assert(UNIT + (WIDTH + 7) / (sizeof(Unit) * 8) <= CAPACITY, "");

	return ConstBits{_storage + UNIT, WIDTH};
}

//------------------------------------------------------------------------------

template <typename TIndex, ShortIndex NCapacity>
typename BitArray<TIndex, NCapacity>::Bits
BitArray<TIndex, NCapacity>::bits(const Units& units) {
	HFSM_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return Bits{_storage + units.unit, units.width};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TIndex, ShortIndex NCapacity>
typename BitArray<TIndex, NCapacity>::ConstBits
BitArray<TIndex, NCapacity>::bits(const Units& units) const {
	HFSM_ASSERT(units.unit + (units.width + 7) / (sizeof(Unit) * 8) <= CAPACITY);

	return ConstBits{_storage + units.unit, units.width};
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
class List {
public:
	using Index = LongIndex;

	static constexpr Index CAPACITY = NCapacity;

	static constexpr Index INVALID = Index (-1);

private:
	using Item  = TItem;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Links {
		Index prev;
		Index next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	union Cell {
		Item item;
		Links links;

		HFSM_INLINE Cell()
			: links{}
		{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	template <typename... TArgs>
	Index emplace(TArgs... args);

	void remove(const Index i);

	HFSM_INLINE		  Item& operator[] (const Index i);
	HFSM_INLINE const Item& operator[] (const Index i) const;

	HFSM_INLINE Index count() const							{ return _count;	}

private:
	HFSM_IF_ASSERT(void verifyStructure(const Index occupied = INVALID) const);

private:
	Cell _cells[CAPACITY];
	Index _vacantHead = 0;
	Index _vacantTail = 0;
	Index _last = 0;
	Index _count = 0;
};

//------------------------------------------------------------------------------

template <typename TItem>
class List<TItem, 0> {};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TItem, LongIndex NCapacity>
template <typename... TArgs>
LongIndex
List<TItem, NCapacity>::emplace(TArgs... args) {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		const Index result = _vacantHead;
		auto& cell = _cells[result];
		++_count;

		if (_vacantHead != _vacantTail) {
			// recycle
			HFSM_ASSERT(cell.links.prev == INVALID);
			HFSM_ASSERT(cell.links.next != INVALID);

			_vacantHead = cell.links.next;

			auto& head = _cells[_vacantHead];
			HFSM_ASSERT(head.links.prev == result);
			head.links.prev = INVALID;
		} else if (_last < CAPACITY) {
			// grow
			++_last;
			_vacantHead = _last;
			_vacantTail = _last;

			auto& nextVacant = _cells[_vacantHead];
			nextVacant.links.prev = INVALID;
			nextVacant.links.next = INVALID;
		} else {
			HFSM_ASSERT(_count == CAPACITY);

			_vacantHead = INVALID;
			_vacantTail = INVALID;
		}

		HFSM_IF_ASSERT(verifyStructure());

		new (&cell.item) Item{std::forward<TArgs>(args)...};

		return result;
	} else {
		// full
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_BREAK();

		return INVALID;
	}
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::remove(const Index i) {
	HFSM_ASSERT(i < CAPACITY && _count);

	auto& fresh = _cells[i];

	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		fresh.links.prev = INVALID;
		fresh.links.next = _vacantHead;

		auto& head = _cells[_vacantHead];
		head.links.prev = i;

		_vacantHead = i;
	} else {
		// 0 -> 1
		HFSM_ASSERT(_count == CAPACITY);
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);

		fresh.links.prev = INVALID;
		fresh.links.next = INVALID;

		_vacantHead = i;
		_vacantTail = i;
	}

	--_count;

	HFSM_IF_ASSERT(verifyStructure());
}

//------------------------------------------------------------------------------

template <typename TItem, LongIndex NCapacity>
TItem&
List<TItem, NCapacity>::operator[] (const Index i) {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TItem, LongIndex NCapacity>
const TItem&
List<TItem, NCapacity>::operator[] (const Index i) const {
	HFSM_IF_ASSERT(verifyStructure());

	return _cells[i].item;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_ASSERT

template <typename TItem, LongIndex NCapacity>
void
List<TItem, NCapacity>::verifyStructure(const Index occupied) const {
	if (_count < CAPACITY) {
		HFSM_ASSERT(_vacantHead < CAPACITY);
		HFSM_ASSERT(_vacantTail < CAPACITY);

		HFSM_ASSERT(_cells[_vacantHead].links.prev == INVALID);
		HFSM_ASSERT(_cells[_vacantTail].links.next == INVALID);

		auto emptyCount = 1;

		for (auto c = _vacantHead; c != _vacantTail; ) {
			HFSM_ASSERT(occupied != c);

			const auto& current = _cells[c];

			const auto f = current.links.next;
			if (f != INVALID) {
				// next
				const auto& following = _cells[f];

				HFSM_ASSERT(following.links.prev == c);

				c = f;
				continue;
			} else {
				// end
				HFSM_ASSERT(_vacantTail == c);
				HFSM_ASSERT(_count == CAPACITY - emptyCount);

				break;
			}
		}
	} else {
		HFSM_ASSERT(_vacantHead == INVALID);
		HFSM_ASSERT(_vacantTail == INVALID);
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////
// http://xoshiro.di.unimi.it/splitmix64.c

class SplitMix64 {
public:
	inline SplitMix64() {}
	inline SplitMix64(const uint64_t seed);

	inline uint64_t next();

private:
	inline uint64_t raw();

private:
	uint64_t _state;
};

//------------------------------------------------------------------------------
// xoshiro256+ (http://xoshiro.di.unimi.it/xoshiro256plus.c)

class XoShiRo256Plus {
public:
	inline XoShiRo256Plus();
	inline XoShiRo256Plus(const uint64_t s);
	inline XoShiRo256Plus(const uint64_t(& s)[4]);

	inline void seed(const uint64_t s);
	inline void seed(const uint64_t(& s)[4]);

	inline float next();
	inline void jump();

private:
	inline uint64_t raw();

private:
	uint64_t _state[4];
};

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/forum/#!topic/prng/VFjdFmbMgZI

class SplitMix32 {
public:
	inline SplitMix32() {}
	inline SplitMix32(const uint32_t seed);

	inline uint32_t next();

private:
	inline uint32_t raw();

private:
	uint32_t _state;
};

//------------------------------------------------------------------------------
// http://xoshiro.di.unimi.it/xoshiro128plus.c

class XoShiRo128Plus {
public:
	inline XoShiRo128Plus();
	inline XoShiRo128Plus(const uint32_t s);
	inline XoShiRo128Plus(const uint32_t(& s)[4]);

	inline void seed(const uint32_t s);
	inline void seed(const uint32_t(& s)[4]);

	inline float next();
	inline void jump();

private:
	inline uint32_t raw();

private:
	uint32_t _state[4];
};

//------------------------------------------------------------------------------

template <typename T>
class RandomT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <>
class RandomT<float>
	: public HFSM_64BIT_OR_32BIT(XoShiRo256Plus, XoShiRo128Plus)
{
public:
	using HFSM_64BIT_OR_32BIT(XoShiRo256Plus::XoShiRo256Plus,
							  XoShiRo128Plus::XoShiRo128Plus);
};

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////
namespace detail {

inline
float
toFloat(const uint32_t uint) {
	constexpr uint32_t mask = (1ULL << 24) - 1;
	const float x = (float) (uint & mask);

	return ldexpf(x, -24);

}

//------------------------------------------------------------------------------

inline
uint32_t
rotl(const uint32_t x, const uint32_t k) {
	return (x << k) | (x >> (32 - k));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline
uint64_t
rotl(const uint64_t x, const uint64_t k) {
	return (x << k) | (x >> (64 - k));
}

}
////////////////////////////////////////////////////////////////////////////////

SplitMix64::SplitMix64(const uint64_t seed)
	: _state{seed}
{}

//------------------------------------------------------------------------------

uint64_t
SplitMix64::next() {
	for (;;)
		if (const uint64_t number = raw())
			return number;
}

//------------------------------------------------------------------------------

uint64_t
SplitMix64::raw() {
	uint64_t z = (_state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	return z ^ (z >> 31);
}

////////////////////////////////////////////////////////////////////////////////

XoShiRo256Plus::XoShiRo256Plus() {
	SplitMix64 generator;

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo256Plus::XoShiRo256Plus(const uint64_t s) {
	seed(s);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo256Plus::XoShiRo256Plus(const uint64_t(& s)[4]) {
	seed(s);
}

//------------------------------------------------------------------------------

void
XoShiRo256Plus::seed(const uint64_t s) {
	SplitMix64 generator{s};

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
XoShiRo256Plus::seed(const uint64_t(& s)[4]) {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

//------------------------------------------------------------------------------

float
XoShiRo256Plus::next() {
	return detail::toFloat((uint32_t) raw());
}

//------------------------------------------------------------------------------

void
XoShiRo256Plus::jump() {
	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;

	for (unsigned i = 0; i < detail::count(JUMP); ++i)
		for (int b = 0; b < 64; ++b) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			raw();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

//------------------------------------------------------------------------------

uint64_t
XoShiRo256Plus::raw() {
	const uint64_t result_plus = _state[0] + _state[3];

	const uint64_t t = _state[1] << 17;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = detail::rotl(_state[3], 45);

	return result_plus;
}

////////////////////////////////////////////////////////////////////////////////

SplitMix32::SplitMix32(const uint32_t seed)
	: _state{seed}
{}

//------------------------------------------------------------------------------

uint32_t
SplitMix32::next() {
	for (;;)
		if (const uint32_t number = raw())
			return number;
}

//------------------------------------------------------------------------------

uint32_t
SplitMix32::raw() {
	uint32_t z = (_state += 0x9E3779B9);
	z = (z ^ (z >> 16)) * 0x85ebca6b;
	z = (z ^ (z >> 13)) * 0xc2b2ae35;

	return z ^ (z >> 16);
}

////////////////////////////////////////////////////////////////////////////////

XoShiRo128Plus::XoShiRo128Plus() {
	SplitMix32 generator;

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo128Plus::XoShiRo128Plus(const uint32_t s) {
	seed(s);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

XoShiRo128Plus::XoShiRo128Plus(const uint32_t(& s)[4]) {
	seed(s);
}

//------------------------------------------------------------------------------

void
XoShiRo128Plus::seed(const uint32_t s) {
	SplitMix32 generator{s};

	_state[0] = generator.next();
	_state[1] = generator.next();
	_state[2] = generator.next();
	_state[3] = generator.next();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void
XoShiRo128Plus::seed(const uint32_t(& s)[4]) {
	_state[0] = s[0];
	_state[1] = s[1];
	_state[2] = s[2];
	_state[3] = s[3];
}

//------------------------------------------------------------------------------

float
XoShiRo128Plus::next() {
	return detail::toFloat(raw());
}

//------------------------------------------------------------------------------

void
XoShiRo128Plus::jump() {
	static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;

	for (unsigned i = 0; i < detail::count(JUMP); ++i)
		for (unsigned b = 0; b < 32; ++b) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= _state[0];
				s1 ^= _state[1];
				s2 ^= _state[2];
				s3 ^= _state[3];
			}
			raw();
		}

	_state[0] = s0;
	_state[1] = s1;
	_state[2] = s2;
	_state[3] = s3;
}

//------------------------------------------------------------------------------

uint32_t
XoShiRo128Plus::raw() {
	const uint32_t result_plus = _state[0] + _state[3];

	const uint32_t t = _state[1] << 9;

	_state[2] ^= _state[0];
	_state[3] ^= _state[1];
	_state[1] ^= _state[2];
	_state[0] ^= _state[3];

	_state[2] ^= t;

	_state[3] = detail::rotl(_state[3], 11);

	return result_plus;
}

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct TL_ {
	static constexpr LongIndex SIZE = sizeof...(Ts);
};

//------------------------------------------------------------------------------

template <typename...>
struct PrependT;

template <typename T, typename... Ts>
struct PrependT<T, TL_<Ts...>> {
	using Type = TL_<T, Ts...>;
};

template <typename... Ts>
using Prepend = typename PrependT<Ts...>::Type;

//------------------------------------------------------------------------------

template <LongIndex, LongIndex, typename...>
struct LesserT;

template <LongIndex H, LongIndex I, typename TFirst, typename... TRest>
struct LesserT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   Prepend<TFirst, typename LesserT<H, I + 1, TRest...>::Type>,
										   typename LesserT<H, I + 1, TRest...>::Type>::type;
};

template <LongIndex H, LongIndex I>
struct LesserT<H, I> {
	using Type = TL_<>;
};

template <typename... Ts>
using SplitL = typename LesserT<sizeof...(Ts) / 2, 0, Ts...>::Type;

//------------------------------------------------------------------------------

template <LongIndex, LongIndex, typename...>
struct GreaterT;

template <LongIndex H, LongIndex I, typename TFirst, typename... TRest>
struct GreaterT<H, I, TFirst, TRest...> {
	using Type = typename std::conditional<(I < H),
										   typename GreaterT<H, I + 1, TRest...>::Type,
										   TL_<TFirst, TRest...>>::type;
};

template <LongIndex H, LongIndex I>
struct GreaterT<H, I> {
	using Type = TL_<>;
};

template <typename... Ts>
using SplitR = typename GreaterT<sizeof...(Ts) / 2, 0, Ts...>::Type;

////////////////////////////////////////////////////////////////////////////////

template<LongIndex N>
struct IndexConstant {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex... Ns>
struct IndexSequence {
	using Type = IndexSequence<Ns...>;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<typename, typename>
struct MakeIndexSequence_Impl {};

template<LongIndex N, LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<N>,
							  IndexSequence<Ns...>>
	: MakeIndexSequence_Impl<IndexConstant<N - 1>,
							 IndexSequence<N - 1, Ns...>>
{};

template<LongIndex... Ns>
struct MakeIndexSequence_Impl<IndexConstant<0>,
							  IndexSequence<Ns...>>
	: IndexSequence<Ns...>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<LongIndex N>
using MakeIndexSequence = typename MakeIndexSequence_Impl<IndexConstant<N>,
														  IndexSequence<>>::Type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template<class... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IndexedTypeList_EntryT {};

template <typename T, LongIndex N>
struct IndexedTypeList_EntryN
	: IndexedTypeList_EntryT<T>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedTypeList_Impl;

template <LongIndex... Ns, typename... Ts>
struct IndexedTypeList_Impl<IndexSequence<Ns...>, Ts...>
	: IndexedTypeList_EntryN<Ts, Ns>...
{
	template <typename T, LongIndex N>
	static constexpr LongIndex select(IndexedTypeList_EntryN<T, N>) { return (LongIndex) N; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... Ts>
struct ITL_
	: private IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>
{
	using Base = IndexedTypeList_Impl<IndexSequenceFor<Ts...>, Ts...>;

	static constexpr LongIndex SIZE = sizeof...(Ts);

	template <typename T>
	static constexpr bool contains() { return std::is_base_of<IndexedTypeList_EntryT<T>, ITL_>::value; }

	template <typename T>
	static constexpr
	typename std::enable_if< std::is_base_of<IndexedTypeList_EntryT<T>, ITL_>::value, LongIndex>::type
	index() {
		return Base::template select<T>(ITL_{});
	}

	template <typename T>
	static constexpr
	typename std::enable_if<!std::is_base_of<IndexedTypeList_EntryT<T>, ITL_>::value, LongIndex>::type
	index() {
		return INVALID_LONG_INDEX;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename...>
struct IndexedT;

template <typename... Ts>
struct IndexedT<TL_<Ts...>> {
	using Type = ITL_<Ts...>;
};

template <typename T>
using Indexed = typename IndexedT<T>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct MergeT;

template <typename... Ts1, typename... Ts2>
struct MergeT<TL_<Ts1...>, TL_<Ts2...>> {
	using TypeList = TL_<Ts1..., Ts2...>;
};

template <typename... Ts>
using Merge = typename MergeT<Ts...>::TypeList;

////////////////////////////////////////////////////////////////////////////////

}
}


////////////////////////////////////////////////////////////////////////////////

namespace hfsm2 {

enum class Method : ShortIndex {
	RANK,
	UTILITY,
	ENTRY_GUARD,
	ENTER,
	REENTER,
	UPDATE,
	REACT,
	EXIT_GUARD,
	EXIT,
	PLAN_SUCCEEDED,
	PLAN_FAILED,

	COUNT
};

enum class Transition : ShortIndex {
	CHANGE,
	RESTART,
	RESUME,
	UTILIZE,
	RANDOMIZE,
	SCHEDULE,

	COUNT
};

enum class StatusEvent : ShortIndex {
	SUCCEEDED,
	FAILED,

	COUNT
};

//------------------------------------------------------------------------------

static inline
const char*
stateName(const std::type_index stateType) {
	const char* const raw = stateType.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;
		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
}

//------------------------------------------------------------------------------

static inline
const char*
methodName(const Method method) {
	switch (method) {
		case Method::RANK:				return "rank";
		case Method::UTILITY:			return "utility";
		case Method::ENTRY_GUARD:		return "entryGuard";
		case Method::ENTER:				return "enter";
		case Method::REENTER:			return "reenter";
		case Method::UPDATE:			return "update";
		case Method::REACT:				return "react";
		case Method::EXIT_GUARD:		return "exitGuard";
		case Method::EXIT:				return "exit";
		case Method::PLAN_SUCCEEDED:	return "planSucceeded";
		case Method::PLAN_FAILED:		return "planFailed";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

//------------------------------------------------------------------------------

static inline
const char*
transitionName(const Transition transition) {
	switch (transition) {
		case Transition::CHANGE:		return "changeTo";
		case Transition::RESTART:		return "restart";
		case Transition::RESUME:		return "resume";
		case Transition::UTILIZE:		return "utilize";
		case Transition::RANDOMIZE:		return "randomize";
		case Transition::SCHEDULE:		return "schedule";

		default:
			HFSM_BREAK();
			return nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////

}

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG

namespace hfsm2 {

////////////////////////////////////////////////////////////////////////////////

template <typename TUtilty = float>
struct LoggerInterfaceT {
	using Utilty	  = TUtilty;

	using Method	  = ::hfsm2::Method;
	using StateID	  = ::hfsm2::StateID;
	using RegionID	  = ::hfsm2::RegionID;
	using Transition  = ::hfsm2::Transition;
	using StatusEvent = ::hfsm2::StatusEvent;

	virtual void recordMethod(const StateID /*origin*/,
							  const Method /*method*/)
	{}

	virtual void recordTransition(const StateID /*origin*/,
								  const Transition /*transition*/,
								  const StateID /*target*/)
	{}

	virtual void recordTaskStatus(const RegionID /*region*/,
								  const StateID /*origin*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordPlanStatus(const RegionID /*region*/,
								  const StatusEvent /*event*/)
	{}

	virtual void recordCancelledPending(const StateID /*origin*/) {}

	virtual void recordUtilityResolution(const StateID /*head*/,
										 const StateID /*prong*/,
										 const Utilty /*utilty*/)
	{}

	virtual void recordRandomResolution(const StateID /*head*/,
										const StateID /*prong*/,
										const Utilty /*utilty*/)
	{}
};

using LoggerInterface = LoggerInterfaceT<float>;

////////////////////////////////////////////////////////////////////////////////

}

#else

namespace hfsm2 {

template <typename = float>
using LoggerInterfaceT = void;

}

#endif

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 2)

struct TaskLink {
	HFSM_INLINE TaskLink(const Transition transition_,
						 const StateID origin_,
						 const StateID destination_)
		: transition{transition_}
		, origin(origin_)
		, destination(destination_)
		, next(INVALID_LONG_INDEX)
	{}

	Transition transition;
	StateID origin		= INVALID_STATE_ID;
	StateID destination	= INVALID_STATE_ID;
	// TODO: add paylaods

	LongIndex prev		= INVALID_LONG_INDEX;
	LongIndex next		= INVALID_LONG_INDEX;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Bounds {
	LongIndex first		= INVALID_LONG_INDEX;
	LongIndex last		= INVALID_LONG_INDEX;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct PlanDataT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayload,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
	static constexpr ShortIndex TASK_CAPACITY = NTaskCapacity;

	using TaskLinks		= List<TaskLink, TASK_CAPACITY>;
	using TasksBounds	= Array<Bounds, RegionList::SIZE>;
	using TasksBits		= BitArray<StateID, StateList::SIZE>;
	using RegionBits	= BitArray<RegionID, RegionList::SIZE>;

	TaskLinks taskLinks;
	TasksBounds tasksBounds;
	TasksBits tasksSuccesses;
	TasksBits tasksFailures;
	RegionBits planExists;

#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const;
	LongIndex verifyPlan(const RegionID stateId) const;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct PlanDataT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   0,
					   NOrthoCount,
					   NOrthoUnits,
					   TPayload,
					   NTaskCapacity>>
{
#ifdef HFSM_ENABLE_ASSERT
	void verifyPlans() const													{}
	LongIndex verifyPlan(const RegionID) const					{ return 0;		}
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM_ENABLE_ASSERT

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlans() const {
	LongIndex planCount = 0;
	for (RegionID id = 0; id < REGION_COUNT; ++id)
		planCount += verifyPlan(id);

	HFSM_ASSERT(taskLinks.count() == planCount);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
LongIndex
PlanDataT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::verifyPlan(const RegionID regionId) const {
	LongIndex length = 0;
	const Bounds& bounds = tasksBounds[regionId];

	if (bounds.first != INVALID_LONG_INDEX) {
		HFSM_ASSERT(bounds.last != INVALID_LONG_INDEX);

		for (auto slow = bounds.first, fast = slow; ; ) {
			++length;
			const TaskLink& task = taskLinks[slow];

			if (slow != bounds.last) {
				HFSM_ASSERT(task.next != INVALID_LONG_INDEX);
				slow = task.next;

				// loop check
				if (fast != INVALID_LONG_INDEX) {
					fast = taskLinks[fast].next;

					if (fast != INVALID_LONG_INDEX) {
						fast = taskLinks[fast].next;
					}

					if (fast != INVALID_LONG_INDEX)
						HFSM_ASSERT(slow != fast);
				}
			} else {
				HFSM_ASSERT(task.next == INVALID_LONG_INDEX);

				break;
			}
		};
	} else
		HFSM_ASSERT(bounds.last == INVALID_LONG_INDEX);

	return length;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct Status {
	enum Result {
		NONE,
		SUCCESS,
		FAILURE
	};

	Result result = NONE;
	bool outerTransition = false;

	inline Status(const Result result_ = NONE,
				  const bool outerTransition_ = false);

	inline explicit operator bool() const { return result || outerTransition; }

	inline void clear();
};

#pragma pack(pop)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Status
combine(const Status lhs, const Status rhs) {
	const Status::Result result = lhs.result > rhs.result ?
		lhs.result : rhs.result;

	return {result, lhs.outerTransition || rhs.outerTransition};
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ConstPlanT {
	template <typename>
	friend class ControlT;

	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	static constexpr LongIndex TASK_CAPACITY = Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HFSM_INLINE Iterator(const ConstPlanT& plan);

		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void operator ++();

		HFSM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HFSM_INLINE LongIndex next() const;

		const ConstPlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE ConstPlanT(const PlanData& planData,
						   const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE Iterator first()			{ return Iterator{*this};								}

private:
	const PlanData& _planData;
	const Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class PlanT {
	template <typename>
	friend class PlanControlT;

	template <typename>
	friend class FullControlT;

	template <typename>
	friend class GuardControlT;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	static constexpr LongIndex  TASK_CAPACITY	= Args::TASK_CAPACITY;

public:
	using PlanData		= PlanDataT<Args>;
	using TaskLinks		= typename PlanData::TaskLinks;
	using TaskIndex		= typename TaskLinks::Index;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Iterator {
		HFSM_INLINE Iterator(PlanT& plan);

		HFSM_INLINE explicit operator bool() const;

		HFSM_INLINE void operator ++();

		HFSM_INLINE		  TaskLink& operator  *()	    { return  _plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink& operator  *() const { return  _plan._planData.taskLinks[_curr];	}

		HFSM_INLINE		  TaskLink* operator ->()	    { return &_plan._planData.taskLinks[_curr];	}
		HFSM_INLINE const TaskLink* operator ->() const { return &_plan._planData.taskLinks[_curr];	}

		HFSM_INLINE void remove();

		HFSM_INLINE LongIndex next() const;

		PlanT& _plan;
		LongIndex _curr;
		LongIndex _next;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE PlanT(PlanData& planData,
					  const RegionID regionId);

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}

	bool append(const Transition transition,
				const StateID origin,
				const StateID destination);

public:
	HFSM_INLINE explicit operator bool() const;

	HFSM_INLINE void clear();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool change   (const StateID origin, const StateID destination)	{ return append(Transition::CHANGE,	   origin, destination); }
	HFSM_INLINE bool restart  (const StateID origin, const StateID destination)	{ return append(Transition::RESTART,   origin, destination); }
	HFSM_INLINE bool resume   (const StateID origin, const StateID destination)	{ return append(Transition::RESUME,	   origin, destination); }
	HFSM_INLINE bool utilize  (const StateID origin, const StateID destination)	{ return append(Transition::UTILIZE,   origin, destination); }
	HFSM_INLINE bool randomize(const StateID origin, const StateID destination)	{ return append(Transition::RANDOMIZE, origin, destination); }
	HFSM_INLINE bool schedule (const StateID origin, const StateID destination)	{ return append(Transition::SCHEDULE,  origin, destination); }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin>
	HFSM_INLINE bool change   (const StateID destination)					{ return change   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool restart  (const StateID destination)					{ return restart  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool resume   (const StateID destination)					{ return resume   (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool utilize  (const StateID destination)					{ return utilize  (stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool randomize(const StateID destination)					{ return randomize(stateId<TOrigin>(), destination);				}

	template <typename TOrigin>
	HFSM_INLINE bool schedule (const StateID destination)					{ return schedule (stateId<TOrigin>(), destination);				}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool change   ()											{ return change   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool restart  ()											{ return restart  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool resume   ()											{ return resume   (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool utilize  ()											{ return utilize  (stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool randomize()											{ return randomize(stateId<TOrigin>(), stateId<TDestination>());	}

	template <typename TOrigin, typename TDestination>
	HFSM_INLINE bool schedule ()											{ return schedule (stateId<TOrigin>(), stateId<TDestination>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Iterator first()			{ return Iterator{*this};								}

private:
	void remove(const LongIndex task);

private:
	PlanData& _planData;
	const RegionID _regionId;
	Bounds& _bounds;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

Status::Status(const Result result_,
			   const bool outerTransition_)
	: result{result_}
	, outerTransition{outerTransition_}
{}

//------------------------------------------------------------------------------

void
Status::clear() {
	result = NONE;
	outerTransition = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ConstPlanT<TArgs>::Iterator::Iterator(const ConstPlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
ConstPlanT<TArgs>::Iterator::operator bool() const {
	HFSM_ASSERT(_curr < ConstPlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < ConstPlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
ConstPlanT<TArgs>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
LongIndex
ConstPlanT<TArgs>::Iterator::next() const {
	if (_curr < ConstPlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._planData.taskLinks[_curr];

		return task.next;
	} else {
		HFSM_ASSERT(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
ConstPlanT<TArgs>::ConstPlanT(const PlanData& planData,
							  const RegionID regionId)

	: _planData{planData}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
ConstPlanT<TArgs>::operator bool() const {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM_ASSERT(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::Iterator::Iterator(PlanT& plan)
	: _plan{plan}
	, _curr{plan._bounds.first}
{
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::Iterator::operator bool() const {
	HFSM_ASSERT(_curr < PlanT::TASK_CAPACITY || _curr == INVALID_LONG_INDEX);

	return _curr < PlanT::TASK_CAPACITY;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::operator ++() {
	_curr = _next;
	_next = next();
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::Iterator::remove() {
	_plan.remove(_curr);
}

//------------------------------------------------------------------------------

template <typename TArgs>
LongIndex
PlanT<TArgs>::Iterator::next() const {
	if (_curr < PlanT::TASK_CAPACITY) {
		const TaskLink& task = _plan._planData.taskLinks[_curr];

		return task.next;
	} else {
		HFSM_ASSERT(_curr == INVALID_LONG_INDEX);

		return INVALID_LONG_INDEX;
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
PlanT<TArgs>::PlanT(PlanData& planData,
					const RegionID regionId)

	: _planData{planData}
	, _regionId{regionId}
	, _bounds{planData.tasksBounds[regionId]}
{}

//------------------------------------------------------------------------------

template <typename TArgs>
PlanT<TArgs>::operator bool() const {
	if (_bounds.first < TASK_CAPACITY) {
		HFSM_ASSERT(_bounds.last < TASK_CAPACITY);
		return true;
	} else {
		HFSM_ASSERT(_bounds.last == INVALID_LONG_INDEX);
		return false;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
bool
PlanT<TArgs>::append(const Transition transition,
					 const StateID origin,
					 const StateID destination)
{
	_planData.planExists.set(_regionId);

	const TaskIndex index = _planData.taskLinks.emplace(transition, origin, destination);
	if (index == TaskLinks::INVALID)
		return false;

	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		auto& last  = _planData.taskLinks[_bounds.last];
		last.next = index;

		auto& next = _planData.taskLinks[index];
		next.prev  = _bounds.last;

		_bounds.last = index;
	} else {
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
					_bounds.last  == INVALID_LONG_INDEX);

		_bounds.first = index;
		_bounds.last  = index;
	}

	return true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::clear() {
	if (_bounds.first < TaskLinks::CAPACITY) {
		HFSM_ASSERT(_bounds.last < TaskLinks::CAPACITY);

		for (LongIndex index = _bounds.first;
			 index != INVALID_LONG_INDEX;
			 )
		{
			HFSM_ASSERT(index < TaskLinks::CAPACITY);

			const auto& task = _planData.taskLinks[index];
			HFSM_ASSERT(index == _bounds.first ?
				   task.prev == INVALID_LONG_INDEX :
				   task.prev <  TaskLinks::CAPACITY);

			const LongIndex next = task.next;

			_planData.taskLinks.remove(index);

			index = next;
		}

		_bounds.first = INVALID_LONG_INDEX;
		_bounds.last  = INVALID_LONG_INDEX;
	} else
		HFSM_ASSERT(_bounds.first == INVALID_LONG_INDEX &&
			   _bounds.last  == INVALID_LONG_INDEX);
}

//------------------------------------------------------------------------------

template <typename TArgs>
void
PlanT<TArgs>::remove(const LongIndex task) {
	HFSM_ASSERT(_planData.planExists.get(_regionId) &&
				_bounds.first < TaskLinks::CAPACITY &&
				_bounds.last  < TaskLinks::CAPACITY);

	HFSM_ASSERT(task < TaskLinks::CAPACITY);

	const TaskLink& curr = _planData.taskLinks[task];

	if (curr.prev < TaskLinks::CAPACITY) {
		TaskLink& prev = _planData.taskLinks[curr.prev];
		prev.next = curr.next;
	} else {
		HFSM_ASSERT(_bounds.first == task);
		_bounds.first = curr.next;
	}

	if (curr.next < TaskLinks::CAPACITY) {
		TaskLink& next = _planData.taskLinks[curr.next];
		next.prev = curr.prev;
	} else {
		HFSM_ASSERT(_bounds.last == task);
		_bounds.last = curr.prev;
	}

	_planData.taskLinks.remove(task);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

enum Strategy {
	Composite,
	Resumable,
	Utilitarian,
	RandomUtil,
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	HFSM_INLINE Parent() = default;

	HFSM_INLINE Parent(const ForkID forkId_)
		: forkId{forkId_}
	{}

	HFSM_INLINE Parent(const ForkID forkId_,
					   const ShortIndex prong_)
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM_INLINE explicit operator bool() const {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT_INDEX;
	}

	ForkID	   forkId = INVALID_FORK_ID;
	ShortIndex prong  = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayload>
struct RequestT {
	using Payload = TPayload;

	enum Type {
		REMAIN,
		CHANGE,
		RESTART,
		RESUME,
		UTILIZE,
		RANDOMIZE,
		SCHEDULE,

		COUNT
	};

	HFSM_INLINE RequestT() = default;

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_,
						 const Payload& payload_)
		: type{type_}
		, stateId{stateId_}
		, payload{payload_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	Type type = CHANGE;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

template <typename TPayload, ShortIndex NCount>
using RequestsT = Array<RequestT<TPayload>, NCount>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCompoCount, LongIndex NOrthoCount, LongIndex NOrthoUnits>
struct AllForksT {
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using Compo = StaticArray<ShortIndex, COMPO_REGIONS>;
	using Ortho = BitArray<ShortIndex, ORTHO_REGIONS>;

	Compo compo{INVALID_SHORT_INDEX};
	Ortho ortho;

	HFSM_INLINE void clear();
};

////////////////////////////////////////////////////////////////////////////////

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct StateRegistryT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							NOrthoCount,
							NOrthoUnits,
							TPayload,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;

	using Request		= RequestT<Payload>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArray<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArray<Units,  ORTHO_REGIONS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using AllForks		= AllForksT<COMPO_REGIONS, ORTHO_REGIONS, ORTHO_UNITS>;
	using OrthoBits		= typename AllForks::Ortho::Bits;

	using CompoRemains	= BitArray<ShortIndex, COMPO_REGIONS>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent&	  forkParent(const ForkID forkId) const;

	HFSM_INLINE OrthoBits resumableOrthoFork(const ForkID forkId);
	HFSM_INLINE OrthoBits requestedOrthoFork(const ForkID forkId);

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoActive{INVALID_SHORT_INDEX};
	AllForks resumable;

	AllForks requested;
	CompoRemains compoRemains;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							0,
							0,
							TPayload,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;
	using Request		= RequestT<Payload>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using AllForks		= AllForksT<COMPO_REGIONS, 0, 0>;
	using CompoRemains	= BitArray<ShortIndex, COMPO_REGIONS>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoActive{INVALID_SHORT_INDEX};
	AllForks resumable;

	AllForks requested;
	CompoRemains compoRemains;
};

////////////////////////////////////////////////////////////////////////////////

}
}


namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCC, LongIndex NOC, LongIndex NOU>
void
AllForksT<NCC, NOC, NOU>::clear() {
	compo.fill(INVALID_SHORT_INDEX);
	ortho.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isActive(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == compoActive[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong == resumable.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return requested.compo[parent.forkId - 1] !=
						   compoActive[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong !=	   compoActive[parent.forkId - 1] &&
					   parent.prong == requested.compo[parent.forkId - 1];
		}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		for (Parent parent = stateParents[stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			HFSM_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return parent.prong ==	   compoActive[parent.forkId - 1] &&
					   parent.prong != requested.compo[parent.forkId - 1];
		}

	return true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId != 0);

	return forkId > 0 ?
		compoParents[ forkId - 1] :
		orthoParents[-forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
typename StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::OrthoBits
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::resumableOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return resumable.ortho.bits(units);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
typename StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::OrthoBits
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestedOrthoFork(const ForkID forkId) {
	HFSM_ASSERT(forkId < 0);
	const Units& units = orthoUnits[-forkId - 1];

	return requested.ortho.bits(units);
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent;

		for (parent = stateParents[request.stateId];
			 parent;
			 parent = forkParent(parent.forkId))
		{
			if (parent.forkId > 0) {
				requested.compo[parent.forkId - 1] = parent.prong;
				parent = forkParent(parent.forkId);

				break;
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0) {
				compoRemains.set(parent.forkId - 1);

				if (	compoActive[parent.forkId - 1] != parent.prong)
					requested.compo[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			} else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}

		for (; parent; parent = forkParent(parent.forkId)) {
			if (parent.forkId > 0)
				compoRemains.set(parent.forkId - 1);
			else if (parent.forkId < 0)
				requestedOrthoFork(parent.forkId).set(parent.prong);
			else
				HFSM_BREAK();
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::requestScheduled(const StateID stateId) {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		if (parent.forkId > 0)
			resumable.compo[parent.forkId - 1] = parent.prong;
		else if (parent.forkId < 0)
			resumableOrthoFork(parent.forkId).set(parent.prong);
		else
			HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, LongIndex NOC, LongIndex NOU, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, NOC, NOU, TPL, NTC>>::clearRequests() {
	compoRemains.clear();
	requested.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isActive(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive[parent.forkId - 1];
		} else
			return true;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isResumable(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong == resumable.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingChange(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return requested.compo[parent.forkId - 1] !=
					   compoActive[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingEnter(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong !=	   compoActive[parent.forkId - 1] &&
				   parent.prong == requested.compo[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::isPendingExit(const StateID stateId) const {
	if (HFSM_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM_ASSERT(parent.forkId > 0);

			return parent.prong ==	   compoActive[parent.forkId - 1] &&
				   parent.prong != requested.compo[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
const Parent&
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::forkParent(const ForkID forkId) const {
	HFSM_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
bool
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestImmediate(const Request request) {
	if (request.stateId == 0)
		return false;
	else if (HFSM_CHECKED(request.stateId < STATE_COUNT)) {
		Parent parent = stateParents[request.stateId];

		if (HFSM_CHECKED(parent)) {
			HFSM_ASSERT(parent.forkId > 0);

			requested.compo[parent.forkId - 1] = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);

				if (	compoActive[parent.forkId - 1] != parent.prong)
					requested.compo[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			}

			for (; parent; parent = forkParent(parent.forkId)) {
				HFSM_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);
			}
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::requestScheduled(const StateID stateId) {
	HFSM_ASSERT(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT) {
		const Parent parent = stateParents[stateId];

		if (HFSM_CHECKED(parent.forkId > 0))
			resumable.compo[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TSL, typename TRL, LongIndex NCC, typename TPL, LongIndex NTC>
void
StateRegistryT<ArgsT<TC, TG, TSL, TRL, NCC, 0, 0, TPL, NTC>>::clearRequests() {
	compoRemains.clear();
	requested.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class ControlT {
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using Random_		= typename Args::Random_;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

public:
	using StateRegistry	= StateRegistryT<Args>;

	using PlanData		= PlanDataT<Args>;
	using ConstPlan		= ConstPlanT<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(ControlT& control,
						   const RegionID id);

		HFSM_INLINE ~Region();

		ControlT& control;
		const RegionID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE ControlT(Context& context,
						 Random_& random,
						 StateRegistry& stateRegistry,
						 PlanData& planData,
						 Logger* const HFSM_IF_LOGGER(logger))
		: _context{context}
		, _random{random}
		, _stateRegistry{stateRegistry}
		, _planData{planData}
		HFSM_IF_LOGGER(, _logger{logger})
	{}


	HFSM_INLINE void setRegion(const RegionID id);
	HFSM_INLINE void resetRegion(const RegionID id);

public:
	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	HFSM_INLINE Context& _()										{ return _context;							}
	HFSM_INLINE Context& context()									{ return _context;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID id) const			{ return _stateRegistry.isActive   (id);	}
	HFSM_INLINE bool isResumable(const StateID id) const			{ return _stateRegistry.isResumable(id);	}

	HFSM_INLINE bool isScheduled(const StateID id) const			{ return isResumable(id);					}

	template <typename TState>
	HFSM_INLINE bool isActive() const								{ return isActive	(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isResumable() const							{ return isResumable(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const							{ return isResumable(stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE const ConstPlan plan() const						{ return ConstPlan{_planData, _regionId};	}
	HFSM_INLINE const ConstPlan plan(const RegionID id) const		{ return ConstPlan{_planData, id};			}

protected:
#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	HFSM_INLINE Logger* logger()									{ return _logger;							}
#endif

protected:
	Context& _context;
	Random_& _random;
	StateRegistry& _stateRegistry;
	PlanData& _planData;
	RegionID _regionId = 0;
	HFSM_IF_LOGGER(Logger* _logger);
};

//------------------------------------------------------------------------------

template <typename TArgs>
class PlanControlT
	: public ControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Context		= typename Args::Context;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

	using PlanData		= PlanDataT<Args>;

public:
	using Plan			= PlanT<Args>;
	using ConstPlan		= ConstPlanT<Args>;

protected:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Origin {
		HFSM_INLINE Origin(PlanControlT& control_,
						   const StateID id);

		HFSM_INLINE ~Origin();

		PlanControlT& control;
		const StateID prevId;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Region {
		HFSM_INLINE Region(PlanControlT& control,
						   const RegionID id,
						   const StateID index,
						   const LongIndex size);

		HFSM_INLINE ~Region();

		PlanControlT& control;
		const RegionID prevId;
		const LongIndex prevIndex;
		const LongIndex prevSize;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Control::Control;

	HFSM_INLINE void setOrigin  (const StateID id);
	HFSM_INLINE void resetOrigin(const StateID id);

	HFSM_INLINE void setRegion  (const RegionID id, const StateID index, const LongIndex size);
	HFSM_INLINE void resetRegion(const RegionID id, const StateID index, const LongIndex size);

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using Control::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Plan plan()								{ return Plan{_planData, _regionId};					}
	HFSM_INLINE Plan plan(const RegionID id)			{ return Plan{_planData, id};							}

	template <typename TRegion>
	HFSM_INLINE Plan plan()								{ return Plan{_planData, regionId<TRegion>()};			}

	template <typename TRegion>
	HFSM_INLINE Plan plan() const						{ return Plan{_planData, regionId<TRegion>()};			}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE		  PlanData&	planData()				{ return _planData;										}
	HFSM_INLINE const PlanData&	planData() const		{ return _planData;										}

protected:
	using Control::_planData;
	using Control::_regionId;
	HFSM_IF_LOGGER(using Control::_logger);

	StateID _originId = 0;
	StateID _regionIndex = 0;
	LongIndex _regionSize = StateList::SIZE;
	Status _status;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class FullControlT
	: public PlanControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename, Strategy, typename, typename...>
	friend struct C_;

	template <typename, typename, typename, typename...>
	friend struct O_;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using Random_		= typename Args::Random_;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using Origin		= typename PlanControl::Origin;
	using Plan			= typename PlanControl::Plan;
	using PlanData		= PlanDataT<Args>;

	using Request		= RequestT <Payload>;
	using Requests		= RequestsT<Payload, Args::COMPO_REGIONS>;

protected:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Lock {
		HFSM_INLINE Lock(FullControlT& control_);
		HFSM_INLINE ~Lock();

		FullControlT* const control;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	HFSM_INLINE FullControlT(Context& context,
							 Random_& random,
							 StateRegistry& stateRegistry,
							 PlanData& planData,
							 Requests& requests,
							 Logger* const logger)
		: PlanControl{context, random, stateRegistry, planData, logger}
		, _requests{requests}
	{}

	template <typename TState>
	Status updatePlan(TState& headState, const Status subStatus);

	template <typename TState>
	Status buildPlanStatus();

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using PlanControl::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// TODO: test payload versions

	HFSM_INLINE void changeTo (const StateID id);
	HFSM_INLINE void changeTo (const StateID stateId, const Payload& payload);

	HFSM_INLINE void restart  (const StateID id);
	HFSM_INLINE void restart  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void resume	  (const StateID id);
	HFSM_INLINE void resume   (const StateID stateId, const Payload& payload);

	HFSM_INLINE void utilize  (const StateID id);
	HFSM_INLINE void utilize  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void randomize(const StateID id);
	HFSM_INLINE void randomize(const StateID stateId, const Payload& payload);

	HFSM_INLINE void schedule (const StateID id);
	HFSM_INLINE void schedule (const StateID stateId, const Payload& payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	HFSM_INLINE void changeTo ()								{ changeTo (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void changeTo (const Payload& payload)			{ changeTo (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void restart  ()								{ restart  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void restart  (const Payload& payload)			{ restart  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void resume   ()								{ resume   (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void resume	  (const Payload& payload)			{ resume   (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void utilize  ()								{ utilize  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void utilize  (const Payload& payload)			{ utilize  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void randomize()								{ randomize(stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void randomize(const Payload& payload)			{ randomize(stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void schedule ()								{ schedule (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void schedule (const Payload& payload)			{ schedule (stateId<TState>(), payload);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void succeed();
	HFSM_INLINE void fail();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	using PlanControl::_planData;
	using PlanControl::_originId;
	using PlanControl::_regionId;
	using PlanControl::_regionIndex;
	using PlanControl::_regionSize;
	using PlanControl::_status;
	HFSM_IF_LOGGER(using Control::_logger);

	Requests& _requests;
	bool _locked = false;
};

//------------------------------------------------------------------------------

template <typename TArgs>
class GuardControlT
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using Args			= TArgs;
	using Logger		= typename Args::Logger;
	using Context		= typename Args::Context;
	using Random_		= typename Args::Random_;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using PlanData		= PlanDataT<Args>;

	using FullControl	= FullControlT<Args>;

public:
	using Requests		= RequestsT<Payload, Args::COMPO_REGIONS>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	HFSM_INLINE GuardControlT(Context& context,
							  Random_& random,
							  StateRegistry& stateRegistry,
							  PlanData& planData,
							  Requests& requests,
							  const Requests& pendingChanges,
							  Logger* const logger)
		: FullControl{context, random, stateRegistry, planData, requests, logger}
		, _pending{pendingChanges}
	{}

	HFSM_INLINE bool cancelled() const							{ return _cancelled;							}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using Control::isActive;
	using Control::isResumable;
	using Control::isScheduled;

	using PlanControl::plan;

	using Control::_;
	using Control::context;

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isPendingChange(const StateID id) const	{ return _stateRegistry.isPendingChange(id);	}
	HFSM_INLINE bool isPendingEnter	(const StateID id) const	{ return _stateRegistry.isPendingEnter (id);	}
	HFSM_INLINE bool isPendingExit	(const StateID id) const	{ return _stateRegistry.isPendingExit  (id);	}

	template <typename TState>
	HFSM_INLINE bool isPendingChange()							{ return isPendingChange(stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingEnter()							{ return isPendingEnter (stateId<TState>());	}

	template <typename TState>
	HFSM_INLINE bool isPendingExit()							{ return isPendingExit  (stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void cancelPendingTransitions();

	HFSM_INLINE const Requests& pendingTransitions() const		{ return _pending;								}

private:
	using FullControl::_stateRegistry;
	using FullControl::_originId;
	HFSM_IF_LOGGER(using FullControl::_logger);

	bool _cancelled = false;
	const Requests& _pending;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Region::Region(ControlT& control_,
							 const RegionID id)
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(id);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Region::~Region() {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
ControlT<TA>::setRegion(const RegionID id) {
	HFSM_ASSERT(_regionId <= id && id < RegionList::SIZE);

	_regionId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
ControlT<TA>::resetRegion(const RegionID id) { //-V524
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);

	_regionId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
PlanControlT<TA>::Origin::Origin(PlanControlT& control_,
								 const StateID id)
	: control{control_}
	, prevId{control._originId}
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TA>
PlanControlT<TA>::Origin::~Origin() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
PlanControlT<TA>::Region::Region(PlanControlT& control_,
								 const RegionID id,
								 const StateID index,
								 const LongIndex size)
	: control{control_}
	, prevId{control._regionId}
	, prevIndex{control._regionIndex}
	, prevSize{control._regionSize}
{
	control.setRegion(id, index, size);
}

//------------------------------------------------------------------------------

template <typename TA>
PlanControlT<TA>::Region::~Region() {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
PlanControlT<TA>::setOrigin(const StateID id) {
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(_originId <= id && id < StateList::SIZE);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
PlanControlT<TA>::resetOrigin(const StateID id) { //-V524
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(id <= _originId && _originId < StateList::SIZE);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
PlanControlT<TA>::setRegion(const RegionID id,
							const StateID index,
							const LongIndex size)
{
	HFSM_ASSERT(_regionId <= id && id <  RegionList::SIZE);
	HFSM_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
PlanControlT<TA>::resetRegion(const RegionID id, //-V524
							  const StateID index,
							  const LongIndex size)
{
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);
	HFSM_ASSERT(index <= _regionIndex && _regionIndex + _regionSize <= index + size);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
FullControlT<TA>::Lock::Lock(FullControlT& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TA>
FullControlT<TA>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::updatePlan(TState& headState,
							 const Status subStatus)
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM_ASSERT(subStatus);

	if (subStatus.result == Status::FAILURE) {
		_status.result = Status::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return buildPlanStatus<State>();
	} else if (subStatus.result == Status::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, STATE_ID};

					changeTo(it->destination);

					it.remove();
				} else
					break;
			}

			return Status{};
		} else {
			_status.result = Status::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return buildPlanStatus<State>();
		}
	} else
		return Status{};
}

//------------------------------------------------------------------------------

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::buildPlanStatus() {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	switch (_status.result) {
	case Status::NONE:
		HFSM_BREAK();
		break;

	case Status::SUCCESS:
		_planData.tasksSuccesses.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::FAILURE:
		_planData.tasksFailures.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM_BREAK();
	}

	return {_status.result};
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::CHANGE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::CHANGE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId,
						   const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::CHANGE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::CHANGE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::restart(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESTART, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::restart(const StateID stateId,
						  const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESUME, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId,
						 const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::utilize(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::UTILIZE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::UTILIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::utilize(const StateID stateId,
						  const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::UTILIZE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::randomize(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RANDOMIZE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RANDOMIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::randomize(const StateID stateId,
							const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RANDOMIZE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RANDOMIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId) {
	const Request transition{Request::Type::SCHEDULE, stateId};
	_requests << transition;

	HFSM_LOG_TRANSITION(_originId, Transition::SCHEDULE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId,
						   const Payload& payload)
{
	const Request transition{Request::Type::SCHEDULE, stateId, payload};
	_requests << transition;

	HFSM_LOG_TRANSITION(_originId, Transition::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::succeed() {
	_status.result = Status::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::fail() {
	_status.result = Status::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksFailures.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::FAILED);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
GuardControlT<TA>::cancelPendingTransitions() {
	_cancelled = true;

	HFSM_LOG_CANCELLED_PENDING(_originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

namespace hfsm2 {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

//------------------------------------------------------------------------------

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo {
	enum RegionType : ShortIndex {
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	HFSM_INLINE StructureStateInfo(const LongIndex parent_,
								   const RegionType region_,
								   const ShortIndex depth_,
								   const char* const name_)
		: name{name_}
		, parent{parent_}
		, region{region_}
		, depth{depth_}
	{}

	const char* name;
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

template <typename TPayload>
Transition
HFSM_INLINE get(const typename RequestT<TPayload>::Type type) {
	using Request = RequestT<TPayload>;

	switch (type) {
		case Request::CHANGE:
			return Transition::CHANGE;

		case Request::RESTART:
			return Transition::RESTART;

		case Request::RESUME:
			return Transition::RESUME;

		case Request::UTILIZE:
			return Transition::UTILIZE;

		case Request::RANDOMIZE:
			return Transition::RANDOMIZE;

		case Request::SCHEDULE:
			return Transition::SCHEDULE;

		default:
			HFSM_BREAK();
			return Transition::CHANGE;
	}
}

#pragma pack(push, 1)

template <typename TPayload>
struct alignas(4) TransitionInfoT {
	using Payload = TPayload;
	using Request = RequestT<TPayload>;

	HFSM_INLINE TransitionInfoT() = default;

	HFSM_INLINE TransitionInfoT(const Request transition_,
								const Method method_)
		: stateId{transition_.stateId}
		, method{method_}
		, transition{get<Payload>(transition_.type)}
	{
		HFSM_ASSERT(method_ < Method::COUNT);
	}

	StateID stateId = INVALID_STATE_ID;
	Method method;
	Transition transition;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#endif

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class InjectionT {
	template <typename...>
	friend struct B_;

protected:
	using Context		= typename TArgs::Context;
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using StateList		= typename TArgs::StateList;
	using RegionList	= typename TArgs::RegionList;

	using Control		= ControlT<TArgs>;

	using PlanControl	= PlanControlT<TArgs>;
	using Plan			= PlanT<TArgs>;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

public:
	HFSM_INLINE void preEntryGuard(Context&)									{}

	HFSM_INLINE void preEnter	  (Context&)									{}
	HFSM_INLINE void preReenter	  (Context&)									{}

	HFSM_INLINE void preUpdate	  (Context&)									{}

	template <typename TEvent>
	HFSM_INLINE void preReact	  (const TEvent&,
								   Context&)									{}

	HFSM_INLINE void preExitGuard (Context&)									{}

	HFSM_INLINE void postExit	  (Context&)									{}
};

//------------------------------------------------------------------------------

template <typename...>
struct B_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst, typename... TRest>
struct B_<TFirst, TRest...>
	: TFirst
	, B_<TRest...>
{

	HFSM_INLINE void widePreEntryGuard(typename TFirst::Context& context);

	HFSM_INLINE void widePreEnter	  (typename TFirst::Context& context);
	HFSM_INLINE void widePreReenter	  (typename TFirst::Context& context);

	HFSM_INLINE void widePreUpdate	  (typename TFirst::Context& context);

	template <typename TEvent>
	HFSM_INLINE void widePreReact	  (const TEvent& event,
									   typename TFirst::Context& context);

	HFSM_INLINE void widePreExitGuard (typename TFirst::Context& context);

	HFSM_INLINE void widePostExit	  (typename TFirst::Context& context);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TFirst>
struct B_<TFirst>
	: TFirst
{
	HFSM_INLINE typename TFirst::Rank
					  rank			   (const typename TFirst::Control&)		{ return typename TFirst::Rank	 {0};		}

	HFSM_INLINE typename TFirst::Utility
					  utility		   (const typename TFirst::Control&)		{ return typename TFirst::Utility{1.0f};	}

	HFSM_INLINE void  entryGuard	   (typename TFirst::GuardControl&)			{}

	HFSM_INLINE void  enter			   (typename TFirst::PlanControl&)			{}
	HFSM_INLINE void  reenter		   (typename TFirst::PlanControl&)			{}

	HFSM_INLINE void  update		   (typename TFirst::FullControl&)			{}

	template <typename TEvent>
	HFSM_INLINE void  react			   (const TEvent&,
					 				    typename TFirst::FullControl&)			{}

	HFSM_INLINE void  exitGuard		   (typename TFirst::GuardControl&)			{}

	HFSM_INLINE void  exit			   (typename TFirst::PlanControl&)			{}

	HFSM_INLINE void  planSucceeded	   (typename TFirst::FullControl& control)	{ control.succeed();	}
	HFSM_INLINE void  planFailed	   (typename TFirst::FullControl& control)	{ control.fail();		}

	HFSM_INLINE void  widePreEntryGuard(typename TFirst::Context& context);

	HFSM_INLINE void  widePreEnter	   (typename TFirst::Context& context);
	HFSM_INLINE void  widePreReenter   (typename TFirst::Context& context);

	HFSM_INLINE void  widePreUpdate	   (typename TFirst::Context& context);

	template <typename TEvent>
	HFSM_INLINE void  widePreReact	   (const TEvent& event,
					 				    typename TFirst::Context& context);

	HFSM_INLINE void  widePreExitGuard (typename TFirst::Context& context);

	HFSM_INLINE void  widePostExit	   (typename TFirst::Context& context);

	template <typename T>
	static constexpr StateID  stateId()				{ return			typename TFirst::StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()			{ return (RegionID) typename TFirst::RegionList::template index<T>();	}
};

template <typename TArgs>
using Empty = B_<InjectionT<TArgs>>;

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEntryGuard(typename TF::Context& context) {
	TF::preEntryGuard(context);
	B_<TR...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreEnter(typename TF::Context& context) {
	TF::preEnter(context);
	B_<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreReenter(typename TF::Context& context) {
	TF::preReenter(context);
	B_<TR...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreUpdate(typename TF::Context& context) {
	TF::preUpdate(context);
	B_<TR...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
template <typename TEvent>
void
B_<TF, TR...>::widePreReact(const TEvent& event,
							typename TF::Context& context)
{
	TF::preReact(event, context);
	B_<TR...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePreExitGuard(typename TF::Context& context) {
	TF::preExitGuard(context);
	B_<TR...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF, typename... TR>
void
B_<TF, TR...>::widePostExit(typename TF::Context& context) {
	TF::postExit(context);
	B_<TR...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
void
B_<TF>::widePreEntryGuard(typename TF::Context& context) {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreEnter(typename TF::Context& context) {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
void
B_<TF>::widePreReenter(typename TF::Context& context) {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreUpdate(typename TF::Context& context) {
	TF::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
void
B_<TF>::widePreReact(const TEvent& event,
					 typename TF::Context& context)
{
	TF::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePreExitGuard(typename TF::Context& context) {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
void
B_<TF>::widePostExit(typename TF::Context& context) {
	TF::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct SI_;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <Strategy, typename, typename...>
struct CI_;

template <typename...>
struct CSI_;

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct CSI_<TInitial>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename...>
struct OI_;

template <typename...>
struct OSI_;

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...>;

template <typename TInitial>
struct OSI_<TInitial>;

//------------------------------------------------------------------------------

template <typename...>
struct WrapT;

template <typename TH>
struct WrapT<TH> {
	using Type = SI_<TH>;
};

template <Strategy TG, typename TH, typename... TS>
struct WrapT<	 CI_<TG, TH, TS...>> {
	using Type = CI_<TG, TH, TS...>;
};

template <typename... TS>
struct WrapT<	 OI_<TS...>> {
	using Type = OI_<TS...>;
};

template <typename... TS>
using Wrap = typename WrapT<TS...>::Type;

//------------------------------------------------------------------------------

template <typename THead>
struct SI_ final {
	using Head				= THead;
	using StateList			= TL_<Head>;
	using RegionList		= TL_<>;

	static constexpr ShortIndex WIDTH		  = 1;
	static constexpr LongIndex  REVERSE_DEPTH = 1;
	static constexpr ShortIndex COMPO_REGIONS = 0;
	static constexpr LongIndex  COMPO_PRONGS  = 0;
	static constexpr ShortIndex ORTHO_REGIONS = 0;
	static constexpr ShortIndex ORTHO_UNITS	  = 0;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct CSI_<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= CSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH,  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS =		Initial::COMPO_REGIONS + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  =		Initial::COMPO_PRONGS  + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS =		Initial::ORTHO_REGIONS + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS   + Remaining::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <typename TInitial>
struct CSI_<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS = Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

template <Strategy TStrategy, typename THead, typename... TSubStates>
struct CI_ final {
	static constexpr Strategy STRATEGY = TStrategy;

	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= CSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex	WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex	REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex	COMPO_REGIONS = SubStates::COMPO_REGIONS + 1;
	static constexpr LongIndex	COMPO_PRONGS  = SubStates::COMPO_PRONGS + WIDTH;
	static constexpr ShortIndex	ORTHO_REGIONS = SubStates::ORTHO_REGIONS;
	static constexpr ShortIndex	ORTHO_UNITS	  = SubStates::ORTHO_UNITS;

	static constexpr LongIndex	STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex	REGION_COUNT  = RegionList::SIZE;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TInitial, typename... TRemaining>
struct OSI_<TInitial, TRemaining...> {
	using Initial			= Wrap<TInitial>;
	using Remaining			= OSI_<TRemaining...>;
	using StateList			= Merge<typename Initial::StateList,  typename Remaining::StateList>;
	using RegionList		= Merge<typename Initial::RegionList, typename Remaining::RegionList>;

	static constexpr LongIndex  REVERSE_DEPTH = Max<Initial::REVERSE_DEPTH,  Remaining::REVERSE_DEPTH>::VALUE;
	static constexpr ShortIndex COMPO_REGIONS =		Initial::COMPO_REGIONS + Remaining::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  =		Initial::COMPO_PRONGS  + Remaining::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS =		Initial::ORTHO_REGIONS + Remaining::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  =		Initial::ORTHO_UNITS   + Remaining::ORTHO_UNITS;
};

template <typename TInitial>
struct OSI_<TInitial> {
	using Initial			= Wrap<TInitial>;
	using StateList			= typename Initial::StateList;
	using RegionList		= typename Initial::RegionList;

	static constexpr LongIndex  REVERSE_DEPTH = Initial::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS = Initial::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = Initial::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = Initial::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS	  = Initial::ORTHO_UNITS;
};

template <typename THead, typename... TSubStates>
struct OI_ final {
	using Head				= THead;
	using HeadInfo			= SI_<Head>;
	using SubStates			= OSI_<TSubStates...>;
	using StateList			= Merge<typename HeadInfo::StateList, typename SubStates::StateList>;
	using RegionList		= Merge<typename HeadInfo::StateList, typename SubStates::RegionList>;

	static constexpr ShortIndex WIDTH		  = sizeof...(TSubStates);
	static constexpr LongIndex  REVERSE_DEPTH = SubStates::REVERSE_DEPTH + 1;
	static constexpr ShortIndex COMPO_REGIONS = SubStates::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS  = SubStates::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS = SubStates::ORTHO_REGIONS + 1;
	static constexpr ShortIndex ORTHO_UNITS	  = SubStates::ORTHO_UNITS + (WIDTH + 7) / 8;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex REGION_COUNT  = RegionList::SIZE;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct ArgsT final {
	using Context	 = TContext;

	using Config_	 = TConfig;
	using Rank		 = typename Config_::Rank;
	using Utility	 = typename Config_::Utility;
	using Random_	 = typename Config_::Random_;
	using UP		 = typename Config_::UP;
	using Logger	 = typename Config_::Logger;

	using StateList	 = TStateList;
	using RegionList = TRegionList;
	using Payload	 = TPayload;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  TASK_CAPACITY = NTaskCapacity;

	HFSM_IF_STRUCTURE(using StructureStateInfos = Array<StructureStateInfo, STATE_COUNT>);
};

//------------------------------------------------------------------------------

template <StateID NStateID,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  ShortIndex NOrthoUnit>
struct I_ {
	static constexpr StateID	STATE_ID	 = NStateID;
	static constexpr ShortIndex COMPO_INDEX	 = NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	 = NOrthoIndex;
	static constexpr ShortIndex ORTHO_UNIT	 = NOrthoUnit;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename, typename, typename>
struct S_;

template <typename, typename, Strategy, typename, typename...>
struct C_;

template <typename, typename, Strategy, ShortIndex, typename...>
struct CS_;

template <typename, typename, typename, typename...>
struct O_;

template <typename, typename>
class RW_;

//------------------------------------------------------------------------------

template <typename, typename...>
struct MaterialT;

template <typename TN, typename TA, typename TH>
struct MaterialT   <TN, TA, TH> {
	using Type = S_<TN, TA, TH>;
};

template <typename TN, typename TA, Strategy TG, 			 typename... TS>
struct MaterialT   <TN, TA, CI_<TG, void,	   TS...>> {
	using Type = C_<TN, TA,		TG, Empty<TA>, TS...>;
};

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
struct MaterialT   <TN, TA, CI_<TG, TH,	TS...>> {
	using Type = C_<TN, TA,		TG, TH,	TS...>;
};

template <typename TN, typename TA,			  typename... TS>
struct MaterialT   <TN, TA, OI_<void,	   TS...>> {
	using Type = O_<TN, TA,		Empty<TA>, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct MaterialT   <TN, TA, OI_<TH,	 TS...>> {
	using Type = O_<TN, TA,		TH,	 TS...>;
};

template <typename TN, typename... TS>
using Material = typename MaterialT<TN, TS...>::Type;

//------------------------------------------------------------------------------

template <typename TConfig,
		  typename TApex>
struct RF_ final {
	using Config_		= TConfig;
	using Context		= typename Config_::Context;
	using Payload		= typename Config_::Payload;
	using Apex			= TApex;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static constexpr LongIndex SUBSTITUTION_LIMIT= Config_::SUBSTITUTION_LIMIT;

	static constexpr LongIndex TASK_CAPACITY	 = Config_::TASK_CAPACITY != INVALID_LONG_INDEX ?
													   Config_::TASK_CAPACITY : Apex::COMPO_PRONGS * 2;

	static constexpr ShortIndex COMPO_REGIONS	 = Apex::COMPO_REGIONS;
	static constexpr ShortIndex ORTHO_REGIONS	 = Apex::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		 = Apex::ORTHO_UNITS;

	using StateList		= Indexed<typename Apex::StateList>;
	using RegionList	= Indexed<typename Apex::RegionList>;

	using Args			= ArgsT<Context,
								Config_,
								StateList,
								RegionList,
								COMPO_REGIONS,
								ORTHO_REGIONS,
								ORTHO_UNITS,
								Payload,
								TASK_CAPACITY>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Instance		= RW_<Config_, Apex>;

	using Control		= ControlT	   <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Injection		= InjectionT<Args>;

	using State			= Empty<Args>;

	template <typename... TInjections>
	using StateT		= B_<TInjections...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	static constexpr bool contains() {
		return StateList::template index<T>() != INVALID_LONG_INDEX;
	}

	template <typename T>
	static constexpr StateID  stateId()		{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()	{ return (RegionID) RegionList::template index<T>();	}
};

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
struct CSubMaterialT;

//template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
//struct CSubMaterialT	 <TN, TA, TG, NI, TL_<T>> {
//	using Type = Material<TN, TA,			  T>;
//};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
struct CSubMaterialT<TN, TA, TG, NI, TL_<TS...>> {
	using Type = CS_<TN, TA, TG, NI,	 TS...>;
};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
using CSubMaterial = typename CSubMaterialT<TN, TA, TG, NI, TS...>::Type;

//------------------------------------------------------------------------------

template <typename...>
struct InfoT;

template <typename TN, typename TA, typename TH>
struct InfoT<  S_<TN, TA, TH>> {
	using Type = SI_<		 TH>;
};

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
struct InfoT<C_<TN, TA, TG, TH, TS...>> {
	using Type = CI_<	   TG, TH, TS...>;
};

template <typename TN, typename TA, typename TH, typename... TS>
struct InfoT<O_<TN, TA, TH, TS...>> {
	using Type = OI_<			   TH, TS...>;
};

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
struct InfoT<CS_<TN, TA, TG, NI, TS...>> {
	using Type = CSI_<				TS...>;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {

//------------------------------------------------------------------------------

struct EmptyContext {};
struct EmptyPayload {};

template <typename TC = EmptyContext,
		  typename TN = char,
		  typename TU = float,
		  typename TG = ::hfsm2::RandomT<TU>,
		  typename TP = EmptyPayload,
		  LongIndex NS = 4,
		  LongIndex NT = INVALID_LONG_INDEX>
struct ConfigT {
	using Context = TC;

	using Rank	  = TN;
	using Utility = TU;
	using Random_ = TG;
	using Logger  = LoggerInterfaceT<Utility>;

	using Payload = TP;

	static constexpr LongIndex SUBSTITUTION_LIMIT = NS;
	static constexpr LongIndex TASK_CAPACITY	  = NT;

	template <typename T>
	using ContextT			 = ConfigT< T, TN, TU, TG, TP, NS, NT>;

	template <typename T>
	using RankT				 = ConfigT<TC,  T, TU, TG, TP, NS, NT>;

	template <typename T>
	using UtilityT			 = ConfigT<TC, TN,  T, TG, TP, NS, NT>;

	template <typename T>
	using RandomT			 = ConfigT<TC, TN, TU,  T, TP, NS, NT>;

	template <typename T>
	using PayloadT			 = ConfigT<TC, TN, TU, TG,  T, NS, NT>;

	template <LongIndex N>
	using SubstitutionLimitN = ConfigT<TC, TN, TU, TG, TP,  N, NS>;

	template <LongIndex N>
	using TaskCapacityN		 = ConfigT<TC, TN, TU, TG, TP, NT,  N>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct UP {
		HFSM_INLINE UP(const Utility utility_  = Utility{1.0f},
					   const ShortIndex prong_ = INVALID_SHORT_INDEX)
			: utility{utility_}
			, prong{prong_}
		{}

		Utility utility;
		ShortIndex prong;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

using Config = ConfigT<>;

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig>
struct M_ {
	using Config_ = TConfig;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Composite			 = CI_<Strategy::Composite,	   THead, TSubStates...>;

	template <				  typename... TSubStates>
	using CompositePeers	 = CI_<Strategy::Composite,    void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Resumable			  = CI_<Strategy::Resumable,   THead, TSubStates...>;

	template <				  typename... TSubStates>
	using ResumablePeers	  = CI_<Strategy::Resumable,   void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Utilitarian		  = CI_<Strategy::Utilitarian, THead, TSubStates...>;

	template <				  typename... TSubStates>
	using UtilitarianPeers	  = CI_<Strategy::Utilitarian, void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Random			  = CI_<Strategy::RandomUtil,  THead, TSubStates...>;

	template <				  typename... TSubStates>
	using RandomPeers		  = CI_<Strategy::RandomUtil,  void,  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using Orthogonal		  = OI_<THead, TSubStates...>;

	template <				  typename... TSubStates>
	using OrthogonalPeers	  = OI_<void,  TSubStates...>;

	//----------------------------------------------------------------------

	template <typename THead, typename... TSubStates>
	using Root				  = RF_<Config_, Composite  <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using PeerRoot			  = RF_<Config_, CompositePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using ResumableRoot		  = RF_<Config_, Resumable  <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using ResumablePeerRoot	  = RF_<Config_, ResumablePeers  <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using UtilitarianRoot	  = RF_<Config_, Utilitarian<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using UtilitarianPeerRoot = RF_<Config_, UtilitarianPeers<  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using RandomRoot		  = RF_<Config_, Random		<THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using RandomPeerRoot	  = RF_<Config_, RandomPeers	 <  TSubStates...>>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename THead, typename... TSubStates>
	using OrthogonalRoot	  = RF_<Config_, Orthogonal <THead, TSubStates...>>;

	template <				  typename... TSubStates>
	using OrthogonalPeerRoot  = RF_<Config_, OrthogonalPeers <  TSubStates...>>;

	//----------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////

}

using Machine = detail::M_<Config>;

template <typename TConfig>
using MachineT = detail::M_<TConfig>;

////////////////////////////////////////////////////////////////////////////////

}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ {
	using Indices		= TIndices;
	static constexpr StateID	STATE_ID	= Indices::STATE_ID;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	using Args			= TArgs;
	using Head			= THead;

	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Logger		= typename Args::Logger;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Empty			= ::hfsm2::detail::Empty<Args>;

	HFSM_INLINE Parent	stateParent			 (Control& control)	{ return control._stateRegistry.stateParents[STATE_ID]; }

	HFSM_INLINE void	deepRegister		 (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl&)				{ return false;	}
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepEnter			 (PlanControl&	control);
	HFSM_INLINE void	deepReenter			 (PlanControl&	control);

	HFSM_INLINE Status	deepUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl&)				{ return false; }
	HFSM_INLINE bool	deepExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepExit			 (PlanControl&	control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wrapPlanSucceeded	 (FullControl&	control);
	HFSM_INLINE void	wrapPlanFailed		 (FullControl&	control);
	HFSM_INLINE Rank	wrapRank			 (Control& control);
	HFSM_INLINE Utility	wrapUtility			 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control&, const RequestType)					{}
	HFSM_INLINE void	deepForwardRequest	 (Control&, const RequestType)					{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control&)										{}
	HFSM_INLINE void	deepRequestRemain	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestRestart	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestResume	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestUtilize	 (Control&)										{}
	HFSM_INLINE void	deepRequestRandomize (Control&)										{}

	HFSM_INLINE UP		deepReportChange	 (Control& control);
	HFSM_INLINE UP		deepReportUtilize	 (Control& control);
	HFSM_INLINE Rank	deepReportRank		 (Control& control);
	HFSM_INLINE Utility	deepReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepEnterRequested	(Control&)										{}
	HFSM_INLINE void   deepChangeToRequested(Control&)										{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Empty>::value;	 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... Ts>
	struct MemberTraits<TReturn(TState::*)(Ts...)> {
		using State = TState;
	};

	template <typename TMethodType, Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(Logger&) const {}

	template <typename TMethodType, Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(Logger& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	// if you see..
	// VS	 - error C2079: 'hfsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	Head _head;
	HFSM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent)
	{
		static constexpr auto HEAD_ID = StateList::template index<TH>();
		StaticAssertEquality<STATE_ID, HEAD_ID>();

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, Empty<TA>> {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;

	static HFSM_INLINE
	void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepRegister(StateRegistry& stateRegistry,
							 const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(stateRegistry.stateParents, parent);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
bool
S_<TN, TA, TH>::deepEntryGuard(GuardControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::entryGuard, Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreEntryGuard(control.context());
	_head.entryGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepEnter(PlanControl& control) {
	HFSM_ASSERT(!control.planData().tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control.planData().tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::enter, Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepReenter(PlanControl& control) {
	HFSM_ASSERT(!control.planData().tasksSuccesses.template get<STATE_ID>());
	HFSM_ASSERT(!control.planData().tasksFailures .template get<STATE_ID>());

	HFSM_LOG_STATE_METHOD(&Head::reenter, Method::REENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReenter(control.context());
	_head.reenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
Status
S_<TN, TA, TH>::deepUpdate(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::update, Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
template <typename TEvent>
Status
S_<TN, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());
	(_head.*reaction)(event, control);				//_head.react(event, control);

	return control._status;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
bool
S_<TN, TA, TH>::deepExitGuard(GuardControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exitGuard, Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreExitGuard(control.context());
	_head.exitGuard(control);

	return !cancelledBefore && control.cancelled();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepExit(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exit, Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	// if you see..
	// VS	 - error C2039:  'exit': is not a member of 'Blah'
	// Clang - error : no member named 'exit' in 'Blah'
	//
	// .. inherit state 'Blah' from hfsm2::Machine::Instance::State
	_head.exit(control);
	_head.widePostExit(control.context());

	control.planData().tasksSuccesses.template reset<STATE_ID>();
	control.planData().tasksFailures .template reset<STATE_ID>();
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::wrapPlanSucceeded(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planSucceeded, Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::wrapPlanFailed(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planFailed, Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planFailed(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
typename TA::Rank
S_<TN, TA, TH>::wrapRank(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::rank, Method::RANK);

	return _head.rank(static_cast<const Control&>(control));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
typename TA::Utility
S_<TN, TA, TH>::wrapUtility(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::utility, Method::UTILITY);

	return _head.utility(static_cast<const Control&>(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
typename TA::UP
S_<TN, TA, TH>::deepReportChange(Control& control) {
	const Utility utility = wrapUtility(control);

	const Parent parent = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
typename TA::UP
S_<TN, TA, TH>::deepReportUtilize(Control& control) {
	const Utility utility = wrapUtility(control);
	const Parent  parent  = stateParent(control);

	return {utility, parent.prong};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
typename TA::Rank
S_<TN, TA, TH>::deepReportRank(Control& control) {
	return wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH>
typename TA::Utility
S_<TN, TA, TH>::deepReportRandomize(Control& control) {
	return wrapUtility(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, typename TH>
const char*
S_<TN, TA, TH>::name() {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH>
void
S_<TN, TA, TH>::deepGetNames(const LongIndex parent,
							  const RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  ShortIndex NIndex,
		  typename... TStates>
struct CS_ {
	static_assert(sizeof...(TStates) >= 2, "");

	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	static constexpr ShortIndex L_PRONG = PRONG_INDEX;

	using LStates		= SplitL<TStates...>;
	using LHalf			= CSubMaterial<I_<INITIAL_ID,
										  COMPO_INDEX,
										  ORTHO_INDEX,
										  ORTHO_UNIT>,
									   Args,
									   STRATEGY,
									   L_PRONG,
									   LStates>;
	using LHalfInfo		= typename InfoT<LHalf>::Type;

	static constexpr ShortIndex R_PRONG = PRONG_INDEX + LStates::SIZE;

	using RStates		= SplitR<TStates...>;
	using RHalf			= CSubMaterial<I_<INITIAL_ID  + LHalfInfo::STATE_COUNT,
										  COMPO_INDEX + LHalfInfo::COMPO_REGIONS,
										  ORTHO_INDEX + LHalfInfo::ORTHO_REGIONS,
										  ORTHO_UNIT  + LHalfInfo::ORTHO_UNITS>,
									   Args,
									   STRATEGY,
									   R_PRONG,
									   RStates>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	wideEntryGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	wideEnter					  (PlanControl& control,							const ShortIndex prong);
	HFSM_INLINE void	wideReenter					  (PlanControl& control,							const ShortIndex prong);

	HFSM_INLINE Status	wideUpdate					  (FullControl& control,							const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact					  (FullControl& control, const TEvent& event,		const ShortIndex prong);

	HFSM_INLINE bool	wideForwardExitGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	wideExitGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	wideExit					  (PlanControl& control,							const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive			  (Control& control,	 const RequestType request,	const ShortIndex prong);
	HFSM_INLINE void	wideForwardRequest			  (Control& control,	 const RequestType request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy TG = STRATEGY>
	HFSM_INLINE void	wideRequestChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE void	wideRequestChange<Composite>  (Control& control,	 const ShortIndex)			{ wideRequestChangeComposite(control);			}

	template <>
	HFSM_INLINE	void	wideRequestChange<Resumable>  (Control& control,	 const ShortIndex prong)	{ wideRequestChangeResumable(control, prong);	}

#else

	HFSM_INLINE void	wideRequestChange			  (Control& control,	 const ShortIndex = INVALID_SHORT_INDEX);

#endif

	HFSM_INLINE void	wideRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	wideRequestChangeResumable	  (Control& control,	 const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume			  (StateRegistry& stateRegistry, const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		wideReportChangeResumable	  (Control& control,	 const ShortIndex prong);
	HFSM_INLINE UP		wideReportChangeUtilitarian	  (Control& control);
	HFSM_INLINE Utility	wideReportChangeRandom		  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportUtilize			  (Control& control);
	HFSM_INLINE Rank	wideReportRank				  (Control& control,	 Rank*	  const ranks);
	HFSM_INLINE Utility	wideReportRandomize			  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested			  (PlanControl& control, const ShortIndex prong);
	HFSM_INLINE void	wideChangeToRequested		  (PlanControl& control, const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = LHalf::NAME_COUNT + RHalf::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	LHalf lHalf;
	RHalf rHalf;
};

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  ShortIndex NIndex,
		  typename TState>
struct CS_<TIndices, TArgs, TStrategy, NIndex, TState> {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using State			= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TState>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	wideEntryGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	wideEnter					  (PlanControl& control,							const ShortIndex prong);
	HFSM_INLINE void	wideReenter					  (PlanControl& control,							const ShortIndex prong);

	HFSM_INLINE Status	wideUpdate					  (FullControl& control,							const ShortIndex prong);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact					  (FullControl& control, const TEvent& event,		const ShortIndex prong);

	HFSM_INLINE bool	wideForwardExitGuard		  (GuardControl& control,							const ShortIndex prong);
	HFSM_INLINE bool	wideExitGuard				  (GuardControl& control,							const ShortIndex prong);

	HFSM_INLINE void	wideExit					  (PlanControl& control,							const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive			  (Control& control,	 const RequestType request,	const ShortIndex prong);
	HFSM_INLINE void	wideForwardRequest			  (Control& control,	 const RequestType request,	const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	wideRequestChangeResumable	  (Control& control,	 const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume			  (StateRegistry& stateRegistry, const ShortIndex prong);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		wideReportChangeResumable	  (Control& control,	 const ShortIndex prong);
	HFSM_INLINE UP		wideReportChangeUtilitarian	  (Control& control);
	HFSM_INLINE Utility	wideReportChangeRandom		  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		wideReportUtilize			  (Control& control);
	HFSM_INLINE Rank	wideReportRank				  (Control& control,	 Rank*	  const ranks);
	HFSM_INLINE Utility	wideReportRandomize			  (Control& control,	 Utility* const options, const Rank* const ranks, const Rank top);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested			  (PlanControl& control, const ShortIndex prong);
	HFSM_INLINE void	wideChangeToRequested		  (PlanControl& control, const ShortIndex prong);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = State::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	State state;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRegister(StateRegistry& stateRegistry,
										 const Parent parent)
{
	lHalf.wideRegister(stateRegistry, Parent{parent.forkId, L_PRONG});
	rHalf.wideRegister(stateRegistry, Parent{parent.forkId, R_PRONG});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideForwardEntryGuard(GuardControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardEntryGuard(control, prong);
	else
		return rHalf.wideForwardEntryGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideEntryGuard(GuardControl& control,
										   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideEntryGuard(control, prong);
	else
		return rHalf.wideEntryGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideEnter(PlanControl& control,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideEnter(control, prong);
	else
		rHalf.wideEnter(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideReenter(PlanControl& control,
										const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideReenter(control, prong);
	else
		rHalf.wideReenter(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
Status
CS_<TN, TA, TG, NI, TS...>::wideUpdate(FullControl& control,
									   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideUpdate(control, prong) :
		rHalf.wideUpdate(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
template <typename TEvent>
Status
CS_<TN, TA, TG, NI, TS...>::wideReact(FullControl& control,
									  const TEvent& event,
									  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	return prong < R_PRONG ?
		lHalf.wideReact(control, event, prong) :
		rHalf.wideReact(control, event, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideForwardExitGuard(GuardControl& control,
												 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideForwardExitGuard(control, prong);
	else
		return rHalf.wideForwardExitGuard(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
bool
CS_<TN, TA, TG, NI, TS...>::wideExitGuard(GuardControl& control,
										  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideExitGuard(control, prong);
	else
		return rHalf.wideExitGuard(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideExit(PlanControl& control,
									 const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideExit(control, prong);
	else
		rHalf.wideExit(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideForwardActive(Control& control,
											  const RequestType request,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardActive(control, request, prong);
	else
		rHalf.wideForwardActive(control, request, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideForwardRequest(Control& control,
											   const RequestType request,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideForwardRequest(control, request, prong);
	else
		rHalf.wideForwardRequest(control, request, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestChangeComposite(Control& control) {
	lHalf.wideRequestChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestChangeResumable(Control& control,
													   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestChangeResumable(control, prong);
	else
		rHalf.wideRequestChangeResumable(control, prong);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestRemain(StateRegistry& stateRegistry) {
	lHalf.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestRestart(StateRegistry& stateRegistry) {
	lHalf.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideRequestResume(StateRegistry& stateRegistry,
											  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideRequestResume(stateRegistry, prong);
	else
		rHalf.wideRequestResume(stateRegistry, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportUtilize(Control& control) {
	const UP l = lHalf.wideReportUtilize(control);
	const UP r = rHalf.wideReportUtilize(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Rank
CS_<TN, TA, TG, NI, TS...>::wideReportRank(Control& control,
										   Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	const Rank l = lHalf.wideReportRank(control, ranks);
	const Rank r = rHalf.wideReportRank(control, ranks + LStates::SIZE);

	return l > r?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Utility
CS_<TN, TA, TG, NI, TS...>::wideReportRandomize(Control& control,
												Utility* const options,
												const Rank* const ranks,
												const Rank top)
{
	HFSM_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportRandomize(control, options,				  ranks,				 top);
	const Utility r = rHalf.wideReportRandomize(control, options + LStates::SIZE, ranks + LStates::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeComposite(Control& control) {
	return lHalf.wideReportChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeResumable(Control& control,
													  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		return lHalf.wideReportChangeResumable(control, prong);
	else
		return rHalf.wideReportChangeResumable(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::UP
CS_<TN, TA, TG, NI, TS...>::wideReportChangeUtilitarian(Control& control) {
	const UP l = lHalf.wideReportChangeUtilitarian(control);
	const UP r = rHalf.wideReportChangeUtilitarian(control);

	return l.utility >= r.utility ?
		l : r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
typename TA::Utility
CS_<TN, TA, TG, NI, TS...>::wideReportChangeRandom(Control& control,
												   Utility* const options,
												   const Rank* const ranks,
												   const Rank top)
{
	HFSM_ASSERT(options && ranks);

	const Utility l = lHalf.wideReportChangeRandom(control, options,				 ranks,					top);
	const Utility r = rHalf.wideReportChangeRandom(control, options + LStates::SIZE, ranks + LStates::SIZE, top);

	return { l + r };
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideEnterRequested(PlanControl& control,
											   const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideEnterRequested(control, prong);
	else
		rHalf.wideEnterRequested(control, prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideChangeToRequested(PlanControl& control,
												  const ShortIndex prong)
{
	HFSM_ASSERT(prong != INVALID_SHORT_INDEX);

	if (prong < R_PRONG)
		lHalf.wideChangeToRequested(control, prong);
	else
		rHalf.wideChangeToRequested(control, prong);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename... TS>
void
CS_<TN, TA, TG, NI, TS...>::wideGetNames(const LongIndex parent,
										 const RegionType /*region*/,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	lHalf.wideGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	rHalf.wideGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRegister(StateRegistry& stateRegistry,
									 const Parent parent)
{
	state.deepRegister(stateRegistry, Parent{parent.forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideForwardEntryGuard(GuardControl& control,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideEntryGuard(GuardControl& control,
									   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideEnter(PlanControl& control,
								  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideReenter(PlanControl& control,
									const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
Status
CS_<TN, TA, TG, NI, T>::wideUpdate(FullControl& control,
								   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
template <typename TEvent>
Status
CS_<TN, TA, TG, NI, T>::wideReact(FullControl& control,
								  const TEvent& event,
								  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideForwardExitGuard(GuardControl& control,
											 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
bool
CS_<TN, TA, TG, NI, T>::wideExitGuard(GuardControl& control,
									  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideExit(PlanControl& control,
								 const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideForwardActive(Control& control,
										  const RequestType request,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardActive(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideForwardRequest(Control& control,
										   const RequestType request,
										   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepForwardRequest(control, request);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestChangeComposite(Control& control) {
	state.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestChangeResumable(Control& control,
												   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestChange(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestRemain(StateRegistry& stateRegistry) {
	state.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestRestart(StateRegistry& stateRegistry) {
	state.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideRequestResume(StateRegistry& stateRegistry,
										  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportUtilize(Control& control) {
	return state.deepReportUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Rank
CS_<TN, TA, TG, NI, T>::wideReportRank(Control& control,
									   Rank* const ranks)
{
	HFSM_ASSERT(ranks);

	*ranks = state.deepReportRank(control);

	return *ranks;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, TG, NI, T>::wideReportRandomize(Control& control,
											Utility* const options,
											const Rank* const ranks,
											const Rank top)
{
	HFSM_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportRandomize(control) : Utility{0.0f};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeComposite(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeResumable(Control& control,
												  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::UP
CS_<TN, TA, TG, NI, T>::wideReportChangeUtilitarian(Control& control) {
	return state.deepReportChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
typename TA::Utility
CS_<TN, TA, TG, NI, T>::wideReportChangeRandom(Control& control,
											   Utility* const options,
											   const Rank* const ranks,
											   const Rank top)
{
	HFSM_ASSERT(options && ranks);

	*options = (*ranks == top) ?
		state.deepReportChange(control).utility : Utility{0.0f};

	return *options;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideEnterRequested(PlanControl& control,
										   const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideChangeToRequested(PlanControl& control,
											  const ShortIndex HFSM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	state.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, ShortIndex NI, typename T>
void
CS_<TN, TA, TG, NI, T>::wideGetNames(const LongIndex parent,
									 const RegionType /*region*/,
									 const ShortIndex depth,
									 StructureStateInfos& _stateInfos) const
{
	state.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  Strategy TStrategy,
		  typename THead,
		  typename... TSubStates>
struct C_ {
	using Indices		= TIndices;
	static constexpr StateID	HEAD_ID		= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr Strategy	STRATEGY	= TStrategy;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		COMPO_ID	= COMPO_INDEX + 1;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;
	using Plan			= PlanT<Args>;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using Head			= THead;

	using HeadState		= S_<Indices, Args, Head>;
	using SubStates		= CS_<I_<HEAD_ID + 1,
								 COMPO_INDEX + 1,
								 ORTHO_INDEX,
								 ORTHO_UNIT>,
							  Args,
							  STRATEGY,
							  0,
							  TSubStates...>;

	using Info			= CI_<STRATEGY, Head, TSubStates...>;
	static constexpr ShortIndex REGION_SIZE	= Info::STATE_COUNT;

	HFSM_INLINE ShortIndex& compoActive   (StateRegistry& stateRegistry)	{ return stateRegistry.compoActive	  [COMPO_INDEX]; }
	HFSM_INLINE ShortIndex& compoResumable(StateRegistry& stateRegistry)	{ return stateRegistry.resumable.compo[COMPO_INDEX]; }
	HFSM_INLINE ShortIndex& compoRequested(StateRegistry& stateRegistry)	{ return stateRegistry.requested.compo[COMPO_INDEX]; }

	HFSM_INLINE ShortIndex& compoActive   (Control& control)				{ return compoActive   (control._stateRegistry); }
	HFSM_INLINE ShortIndex& compoResumable(Control& control)				{ return compoResumable(control._stateRegistry); }
	HFSM_INLINE ShortIndex& compoRequested(Control& control)				{ return compoRequested(control._stateRegistry); }

	HFSM_INLINE ShortIndex	resolveRandom (Control& control,
										   const Utility(& options)[Info::WIDTH], const Utility sum,
										   const Rank	(& ranks)  [Info::WIDTH], const Rank	top);

	HFSM_INLINE bool	compoRemain		  (Control& control)				{ return control._stateRegistry.compoRemains.template get<COMPO_INDEX>(); }

	HFSM_INLINE void	deepRegister				  (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard		  (GuardControl& control);
	HFSM_INLINE bool	deepEntryGuard				  (GuardControl& control);

	HFSM_INLINE void	deepEnter					  (PlanControl&  control);
	HFSM_INLINE void	deepReenter					  (PlanControl&  control);

	HFSM_INLINE Status	deepUpdate					  (FullControl&  control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact					  (FullControl&  control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard		  (GuardControl& control);
	HFSM_INLINE bool	deepExitGuard				  (GuardControl& control);

	HFSM_INLINE void	deepExit					  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepForwardActive			  (Control& control, const RequestType request);
	HFSM_INLINE void   deepForwardRequest			  (Control& control, const RequestType request);

	HFSM_INLINE void   deepRequest					  (Control& control, const RequestType request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy TG = STRATEGY>
	HFSM_INLINE void	deepRequestChange			  (Control& control, const ShortIndex = INVALID_SHORT_INDEX);

	template <>
	HFSM_INLINE void	deepRequestChange<Composite>  (Control& control, const ShortIndex)	{ deepRequestChangeComposite  (control); }

	template <>
	HFSM_INLINE void	deepRequestChange<Resumable>  (Control& control, const ShortIndex)	{ deepRequestChangeResumable  (control); }

	template <>
	HFSM_INLINE void	deepRequestChange<Utilitarian>(Control& control, const ShortIndex)	{ deepRequestChangeUtilitarian(control); }

	template <>
	HFSM_INLINE void	deepRequestChange<RandomUtil> (Control& control, const ShortIndex)	{ deepRequestChangeRandom	  (control); }

#else

	HFSM_INLINE void	deepRequestChange			  (Control& control);

#endif

	HFSM_INLINE void	deepRequestChangeComposite	  (Control& control);
	HFSM_INLINE void	deepRequestChangeResumable	  (Control& control);
	HFSM_INLINE void	deepRequestChangeUtilitarian  (Control& control);
	HFSM_INLINE void	deepRequestChangeRandom		  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestRemain			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestRestart			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestResume			  (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestUtilize			  (Control& control);
	HFSM_INLINE void	deepRequestRandomize		  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM_EXPLICIT_MEMBER_SPECIALIZATION

	template <Strategy TG = STRATEGY>
	HFSM_INLINE UP		deepReportChange			  (Control& control);

	template <>
	HFSM_INLINE UP		deepReportChange<Composite>   (Control& control)	{ return deepReportChangeComposite  (control); }

	template <>
	HFSM_INLINE UP		deepReportChange<Resumable>   (Control& control)	{ return deepReportChangeResumable  (control); }

	template <>
	HFSM_INLINE UP		deepReportChange<Utilitarian> (Control& control)	{ return deepReportChangeUtilitarian(control); }

	template <>
	HFSM_INLINE UP		deepReportChange<RandomUtil>  (Control& control)	{ return deepReportChangeRandom		(control); }

#else

	HFSM_INLINE UP		deepReportChange			  (Control& control);

#endif

	HFSM_INLINE UP		deepReportChangeComposite	  (Control& control);
	HFSM_INLINE UP		deepReportChangeResumable	  (Control& control);
	HFSM_INLINE UP		deepReportChangeUtilitarian   (Control& control);
	HFSM_INLINE UP		deepReportChangeRandom		  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE UP		deepReportUtilize			  (Control& control);
	HFSM_INLINE Rank	deepReportRank				  (Control& control);
	HFSM_INLINE Utility	deepReportRandomize			  (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepEnterRequested			  (PlanControl& control);
	HFSM_INLINE void	deepChangeToRequested		  (PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT = HeadState::NAME_COUNT + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
ShortIndex
C_<TN, TA, TG, TH, TS...>::resolveRandom(Control& control,
										 const Utility(& options)[Info::WIDTH],
										 const Utility sum,
										 const Rank(& ranks)[Info::WIDTH],
										 const Rank top)
{
	const Utility random = control._random.next();
	HFSM_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (ShortIndex i = 0; i < count(ranks); ++i)
		if (ranks[i] == top) {
			HFSM_ASSERT(options[i] >= 0.0f);

			if (cursor >= options[i])
				cursor -= options[i];
			else {
				HFSM_LOG_RANDOM_RESOLUTION(HEAD_ID, i, random);

				return i;
			}
		}

	HFSM_BREAK();
	return INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
										const Parent parent)
{
	stateRegistry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
bool
C_<TN, TA, TG, TH, TS...>::deepForwardEntryGuard(GuardControl& control) {
	const ShortIndex active	   = compoActive   (control);
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT_INDEX)
		return _subStates.wideForwardEntryGuard(control, active);
	else
		return _subStates.wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
bool
C_<TN, TA, TG, TH, TS...>::deepEntryGuard(GuardControl& control) {
	const ShortIndex requested = compoRequested(control);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepEnter(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;

	requested = INVALID_SHORT_INDEX;

	_subStates.wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepReenter(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  != INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);

	if (active == requested)
		_subStates.wideReenter(control, active);
	else {
		_subStates.wideExit	  (control, active);

		active	  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT_INDEX;

		_subStates.wideEnter  (control, active);
	}

	requested = INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
Status
C_<TN, TA, TG, TH, TS...>::deepUpdate(FullControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control, active);

		if (subStatus.outerTransition)
			return Status{Status::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
template <typename TEvent>
Status
C_<TN, TA, TG, TH, TS...>::deepReact(FullControl& control,
									 const TEvent& event)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates.wideReact(control, event, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
bool
C_<TN, TA, TG, TH, TS...>::deepForwardExitGuard(GuardControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT_INDEX)
		return _subStates.wideForwardExitGuard(control, active);
	else
		return _subStates.wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
bool
C_<TN, TA, TG, TH, TS...>::deepExitGuard(GuardControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepExit(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);

	resumable = active;
	active	  = INVALID_SHORT_INDEX;

	auto plan = control.plan(REGION_ID);
	plan.clear();
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepForwardActive(Control& control,
											 const RequestType request)
{
	HFSM_ASSERT(control._stateRegistry.isActive(HEAD_ID));

	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX) {
		const ShortIndex active = compoActive(control);

		_subStates.wideForwardActive (control, request, active);
	} else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepForwardRequest(Control& control,
											  const RequestType request)
{
	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX)
		deepRequest					 (control, request);
	else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequest(Control& control,
									   const RequestType request)
{
	switch (request) {
	case Request::REMAIN:
		deepRequestRemain	(control._stateRegistry);
		break;

	case Request::CHANGE:
		deepRequestChange	(control);
		break;

	case Request::RESTART:
		deepRequestRestart	(control._stateRegistry);
		break;

	case Request::RESUME:
		deepRequestResume	(control._stateRegistry);
		break;

	case Request::UTILIZE:
		deepRequestUtilize	(control);
		break;

	case Request::RANDOMIZE:
		deepRequestRandomize(control);
		break;

	default:
		HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestChange(Control& control)
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite  (control);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable  (control);
		break;

	case Strategy::Utilitarian:
		deepRequestChangeUtilitarian(control);
		break;

	case Strategy::RandomUtil:
		deepRequestChangeRandom		(control);
		break;

	default:
		HFSM_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.wideRequestChangeResumable(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestChangeUtilitarian(Control& control) {
	const UP s = _subStates.wideReportChangeUtilitarian(control);
	HFSM_ASSERT(s.prong != INVALID_SHORT_INDEX);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestChangeRandom(Control& control) {
	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= compoActive   (stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = compoRequested(stateRegistry);

	requested = 0;

	_subStates.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry) {
	const ShortIndex  resumable = compoResumable(stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.wideRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestUtilize(Control& control) {
	const UP s = _subStates.wideReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepRequestRandomize(Control& control) {
	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);
}

//------------------------------------------------------------------------------

#if !HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportChange(Control& control) {
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	case Strategy::RandomUtil:
		return deepReportChangeRandom	  (control);

	default:
		HFSM_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);
	requested = 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportChangeUtilitarian(Control& control) {
	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeUtilitarian(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportChangeRandom(Control& control) {
	const UP h = _headState.deepReportChange(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, TG, TH, TS...>::deepReportUtilize(Control& control) {
	const UP h = _headState.deepReportUtilize(control);
	const UP s = _subStates.wideReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::Rank
C_<TN, TA, TG, TH, TS...>::deepReportRank(Control& control) {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
typename TA::Utility
C_<TN, TA, TG, TH, TS...>::deepReportRandomize(Control& control) {
	const Utility h = _headState.wrapUtility(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);

	return h * options[requested];
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepEnterRequested(PlanControl& control) {
	ShortIndex& active	  = compoActive	  (control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	active	  = requested;
	requested = INVALID_SHORT_INDEX;

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	ShortIndex& active	  = compoActive	  (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == INVALID_SHORT_INDEX)
		_subStates.wideChangeToRequested(control, active);
	else if (requested != active) {
		_subStates.wideExit	  (control, active);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT_INDEX;

		_subStates.wideEnter  (control, active);
	} else if (compoRemain(control)) {
		_subStates.wideExit	  (control, active);

		requested = INVALID_SHORT_INDEX;

		_subStates.wideEnter  (control, active);
	} else {
		requested = INVALID_SHORT_INDEX;

		_subStates.wideReenter(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy TG, typename TH, typename... TS>
void
C_<TN, TA, TG, TH, TS...>::deepGetNames(const LongIndex parent,
										const RegionType region,
										const ShortIndex depth,
										StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent,					 region,						depth,	   _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, StructureStateInfo::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename, typename, ShortIndex, typename...>
struct OS_;

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct OS_<TIndices, TArgs, NIndex, TInitial, TRemaining...> {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT	   <Args>;
	using PlanControl	= PlanControlT <Args>;
	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using InitialInfo	= Wrap<TInitial>;

	using Remaining		= OS_<I_<INITIAL_ID  + InitialInfo::STATE_COUNT,
								 COMPO_INDEX + InitialInfo::COMPO_REGIONS,
								 ORTHO_INDEX + InitialInfo::ORTHO_REGIONS,
								 ORTHO_UNIT  + InitialInfo::ORTHO_UNITS>,
							  Args,
							  PRONG_INDEX + 1,
							  TRemaining...>;

	using Info	= OSI_<TInitial, TRemaining...>;

	HFSM_INLINE void	wideRegister		 (StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control,						const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl& control);

	HFSM_INLINE void	wideEnter			 (PlanControl& control);
	HFSM_INLINE void	wideReenter			 (PlanControl& control);

	HFSM_INLINE Status	wideUpdate			 (FullControl& control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl& control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control,						const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl& control);

	HFSM_INLINE void	wideExit			 (PlanControl& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const RequestType request,	const ProngConstBits prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const RequestType request,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);
	HFSM_INLINE void	wideRequestRandomize (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);
	HFSM_INLINE Utility	wideReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested	 (PlanControl& control);
	HFSM_INLINE void	wideChangeToRequested(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <typename TIndices,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct OS_<TIndices, TArgs, NIndex, TInitial> {
	using Indices		= TIndices;
	static constexpr StateID	INITIAL_ID	= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ShortIndex PRONG_INDEX	= NIndex;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT		<Args>;
	using PlanControl	= PlanControlT	<Args>;
	using FullControl	= FullControlT	<Args>;
	using GuardControl	= GuardControlT<Args>;

	using Initial		= Material<I_<INITIAL_ID,
									  COMPO_INDEX,
									  ORTHO_INDEX,
									  ORTHO_UNIT>,
								   Args,
								   TInitial>;

	using Info	= OSI_<TInitial>;

	HFSM_INLINE void	wideRegister		 (StateRegistry& stateRegistry, const ForkID forkId);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control,						const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardEntryGuard(GuardControl& control);
	HFSM_INLINE bool	wideEntryGuard		 (GuardControl& control);

	HFSM_INLINE void	wideEnter			 (PlanControl& control);
	HFSM_INLINE void	wideReenter			 (PlanControl& control);

	HFSM_INLINE Status	wideUpdate			 (FullControl& control);

	template <typename TEvent>
	HFSM_INLINE Status	wideReact			 (FullControl& control, const TEvent& event);

	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control,						const ProngConstBits prongs);
	HFSM_INLINE bool	wideForwardExitGuard (GuardControl& control);
	HFSM_INLINE bool	wideExitGuard		 (GuardControl& control);

	HFSM_INLINE void	wideExit			 (PlanControl& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideForwardActive	 (Control& control, const RequestType request,	const ProngConstBits prongs);
	HFSM_INLINE void	wideForwardRequest	 (Control& control, const RequestType request,	const ProngConstBits prongs);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideRequestChange	 (Control& control);
	HFSM_INLINE void	wideRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	wideRequestUtilize	 (Control& control);
	HFSM_INLINE void	wideRequestRandomize (Control& control);

	HFSM_INLINE Utility	wideReportChange	 (Control& control);
	HFSM_INLINE Utility	wideReportUtilize	 (Control& control);
	HFSM_INLINE Utility	wideReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wideEnterRequested	 (PlanControl& control);
	HFSM_INLINE void	wideChangeToRequested(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRegister(StateRegistry& stateRegistry,
										 const ForkID forkId)
{
	initial  .deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
	remaining.wideRegister(stateRegistry, forkId);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control,
												  const ProngConstBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardEntryGuard(control) : false;

	const bool r = remaining.wideForwardEntryGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardEntryGuard(GuardControl& control) {
	const bool i = initial  .deepForwardEntryGuard(control);
	const bool r = remaining.wideForwardEntryGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideEntryGuard(GuardControl& control) {
	const bool i = initial  .deepEntryGuard(control);
	const bool r = remaining.wideEntryGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideReenter(PlanControl& control) {
	initial  .deepReenter(control);
	remaining.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
OS_<TN, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
Status
OS_<TN, TA, NI, TI, TR...>::wideReact(FullControl& control,
									  const TEvent& event)
{
	const auto status =	   initial	.deepReact(control, event);
	return combine(status, remaining.wideReact(control, event));
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control,
												 const ProngConstBits prongs)
{
	const bool i = prongs.get(PRONG_INDEX) ?
				   initial  .deepForwardExitGuard(control) : false;

	const bool r = remaining.wideForwardExitGuard(control, prongs);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideForwardExitGuard(GuardControl& control) {
	const bool i = initial  .deepForwardExitGuard(control);
	const bool r = remaining.wideForwardExitGuard(control);

	return i || r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
bool
OS_<TN, TA, NI, TI, TR...>::wideExitGuard(GuardControl& control) {
	const bool i = initial  .deepExitGuard(control);
	const bool r = remaining.wideExitGuard(control);

	return i || r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideForwardActive(Control& control,
											  const RequestType request,
											  const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial	 .deepForwardActive(control, local);
	remaining.wideForwardActive(control, request, prongs);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideForwardRequest(Control& control,
											   const RequestType request,
											   const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial	 .deepForwardRequest(control, local);
	remaining.wideForwardRequest(control, request, prongs);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestChange(Control& control) {
	initial  .deepRequestChange(control);
	remaining.wideRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial	 .deepRequestRemain(stateRegistry);
	remaining.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial	 .deepRequestRestart(stateRegistry);
	remaining.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestResume(StateRegistry& stateRegistry) {
	initial	 .deepRequestResume(stateRegistry);
	remaining.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestUtilize(Control& control) {
	initial  .deepRequestUtilize(control);
	remaining.wideRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideRequestRandomize(Control& control) {
	initial  .deepRequestRandomize(control);
	remaining.wideRequestRandomize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportChange(Control& control) {
	const UP	  i = initial  .deepReportChange(control);
	const Utility r = remaining.wideReportChange(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportUtilize(Control& control) {
	const UP	  i = initial  .deepReportUtilize(control);
	const Utility r = remaining.wideReportUtilize(control);

	return i.utility + r;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
typename TA::Utility
OS_<TN, TA, NI, TI, TR...>::wideReportRandomize(Control& control) {
	const Utility i = initial  .deepReportRandomize(control);
	const Utility r = remaining.wideReportRandomize(control);

	return i + r;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideEnterRequested(PlanControl& control) {
	initial	 .deepEnterRequested(control);
	remaining.wideEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideChangeToRequested(PlanControl& control) {
	initial	 .deepChangeToRequested(control);
	remaining.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, ShortIndex NI, typename TI, typename... TR>
void
OS_<TN, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										 const ShortIndex depth,
										 StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRegister(StateRegistry& stateRegistry,
								  const ForkID forkId)
{
	initial.deepRegister(stateRegistry, Parent{forkId, PRONG_INDEX});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control,
										   const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardEntryGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardEntryGuard(GuardControl& control) {
	return initial.deepForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideEntryGuard(GuardControl& control) {
	return initial.deepEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideEnter(PlanControl& control) {
	initial.deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideReenter(PlanControl& control) {
	initial.deepReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
Status
OS_<TN, TA, NI, TI>::wideUpdate(FullControl& control) {
	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
Status
OS_<TN, TA, NI, TI>::wideReact(FullControl& control,
									   const TEvent& event)
{
	return initial.deepReact(control, event);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control,
										  const ProngConstBits prongs)
{
	return prongs.get(PRONG_INDEX) ?
		initial.deepForwardExitGuard(control) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideForwardExitGuard(GuardControl& control) {
	return initial.deepForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
bool
OS_<TN, TA, NI, TI>::wideExitGuard(GuardControl& control) {
	return initial.deepExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideExit(PlanControl& control) {
	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardActive(Control& control,
									   const RequestType request,
									   const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardActive(control, local);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideForwardRequest(Control& control,
										const RequestType request,
										const ProngConstBits prongs)
{
	const RequestType local = prongs.get(PRONG_INDEX) ?
		request : Request::REMAIN;

	initial.deepForwardRequest(control, local);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestChange(Control& control) {
	initial.deepRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRemain(StateRegistry& stateRegistry) {
	initial.deepRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRestart(StateRegistry& stateRegistry) {
	initial.deepRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestResume(StateRegistry& stateRegistry) {
	initial.deepRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestUtilize(Control& control) {
	initial.deepRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideRequestRandomize(Control& control) {
	initial.deepRequestRandomize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportChange(Control& control) {
	const UP i = initial.deepReportChange(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportUtilize(Control& control) {
	const UP i = initial.deepReportUtilize(control);

	return i.utility;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
typename TA::Utility
OS_<TN, TA, NI, TI>::wideReportRandomize(Control& control) {
	const Utility i = initial.deepReportRandomize(control);

	return i;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideEnterRequested(PlanControl& control) {
	initial.deepEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideChangeToRequested(PlanControl& control) {
	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, ShortIndex NI, typename TI>
void
OS_<TN, TA, NI, TI>::wideGetNames(const LongIndex parent,
								  const ShortIndex depth,
								  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct O_ final {
	using Indices		= TIndices;
	static constexpr StateID	HEAD_ID		= Indices::STATE_ID;
	static constexpr ShortIndex COMPO_INDEX	= Indices::COMPO_INDEX;
	static constexpr ShortIndex ORTHO_INDEX	= Indices::ORTHO_INDEX;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using Payload		= typename Args::Payload;

	using Head			= THead;

	using Info			= OI_<Head, TSubStates...>;
	static constexpr ShortIndex WIDTH		= Info::WIDTH;
	static constexpr ShortIndex REGION_SIZE	= Info::STATE_COUNT;
	static constexpr ShortIndex ORTHO_UNITS	= Info::ORTHO_UNITS;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;
	using OrthoForks	= typename StateRegistry::AllForks::Ortho;
	using ProngBits		= typename OrthoForks::Bits;
	using ProngConstBits= typename OrthoForks::ConstBits;

	using Control		= ControlT<Args>;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;
	using ScopedRegion	= typename PlanControl::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using GuardControl	= GuardControlT<Args>;

	using HeadState		= S_<Indices, Args, Head>;
	using SubStates		= OS_<I_<HEAD_ID + 1,
								 COMPO_INDEX,
								 ORTHO_INDEX + 1,
								 ORTHO_UNIT + ORTHO_UNITS>,
							  Args,
							  0,
							  TSubStates...>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE ProngBits	   orthoRequested(		StateRegistry& stateRegistry)		{ return stateRegistry.requested.ortho.template bits<ORTHO_UNIT, WIDTH>();	}
	HFSM_INLINE ProngConstBits orthoRequested(const StateRegistry& stateRegistry) const	{ return stateRegistry.requested.ortho.template bits<ORTHO_UNIT, WIDTH>();	}

	HFSM_INLINE ProngBits	   orthoRequested(		Control& control)					{ return orthoRequested(control._stateRegistry);							}
	HFSM_INLINE ProngConstBits orthoRequested(const Control& control) const				{ return orthoRequested(control._stateRegistry);							}

	HFSM_INLINE void	deepRegister		 (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl&	control);
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepEnter			 (PlanControl&	control);
	HFSM_INLINE void	deepReenter			 (PlanControl&	control);

	HFSM_INLINE Status	deepUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl&	control);
	HFSM_INLINE bool	deepExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepExit			 (PlanControl&	control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control& control, const RequestType request);
	HFSM_INLINE void	deepForwardRequest	 (Control& control, const RequestType request);

	HFSM_INLINE void	deepRequest			 (Control& control, const RequestType request);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control& control);
	HFSM_INLINE void	deepRequestRemain	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestRestart	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestResume	 (StateRegistry& stateRegistry);
	HFSM_INLINE void	deepRequestUtilize	 (Control& control);
	HFSM_INLINE void	deepRequestRandomize (Control& control);

	HFSM_INLINE UP		deepReportChange	 (Control& control);
	HFSM_INLINE UP		deepReportUtilize	 (Control& control);
	HFSM_INLINE Rank	deepReportRank		 (Control& control);
	HFSM_INLINE Utility	deepReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepEnterRequested	 (PlanControl& control);
	HFSM_INLINE void	deepChangeToRequested(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRegister(StateRegistry& stateRegistry,
									const Parent parent)
{
	stateRegistry.orthoParents[ORTHO_INDEX] = parent;
	stateRegistry.orthoUnits[ORTHO_INDEX] = Units{ORTHO_UNIT, WIDTH};

	_headState.deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry, ORTHO_ID);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardEntryGuard(GuardControl& control) {
	const ProngConstBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardEntryGuard(control, requested);
	else
		return _subStates.wideForwardEntryGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepEntryGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepEnter(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepReenter(PlanControl& control) {
	ProngBits requested = orthoRequested(control);
	requested.clear();

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);
	_subStates.wideReenter(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
Status
O_<TN, TA, TH, TS...>::deepUpdate(FullControl& control) {
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
template <typename TEvent>
Status
O_<TN, TA, TH, TS...>::deepReact(FullControl& control,
								 const TEvent& event)
{
	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const auto headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};
		_subStates.wideReact(control, event);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control.planData().planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepForwardExitGuard(GuardControl& control) {
	const ProngConstBits requested = orthoRequested(static_cast<const GuardControl&>(control));

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested)
		return _subStates.wideForwardExitGuard(control, requested);
	else
		return _subStates.wideForwardExitGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
bool
O_<TN, TA, TH, TS...>::deepExitGuard(GuardControl& control) {
	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepExit(PlanControl& control) {
	_subStates.wideExit(control);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardActive(Control& control,
										 const RequestType request)
{
	HFSM_ASSERT(control._stateRegistry.isActive(HEAD_ID));

	const ProngConstBits requested = orthoRequested(static_cast<const Control&>(control));
	HFSM_ASSERT(!!requested);

	_subStates.wideForwardActive(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepForwardRequest(Control& control,
										  const RequestType request)
{
	const ProngConstBits requested = orthoRequested(static_cast<const Control&>(control));

	if (requested)
		_subStates.wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequest(Control& control,
								   const RequestType request)
{
	switch (request) {
	case Request::REMAIN:
		deepRequestRemain (control._stateRegistry);
		break;

	case Request::CHANGE:
		deepRequestChange (control);
		break;

	case Request::RESTART:
		deepRequestRestart(control._stateRegistry);
		break;

	case Request::RESUME:
		deepRequestResume (control._stateRegistry);
		break;

	case Request::UTILIZE:
		deepRequestUtilize(control);
		break;

	case Request::RANDOMIZE:
		deepRequestRandomize(control);
		break;

	default:
		HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestChange(Control& control) {
	_subStates.wideRequestChange(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRemain(StateRegistry& stateRegistry) {
	_subStates.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRestart(StateRegistry& stateRegistry) {
	_subStates.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestResume(StateRegistry& stateRegistry) {
	_subStates.wideRequestResume(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestUtilize(Control& control) {
	_subStates.wideRequestUtilize(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepRequestRandomize(Control& control) {
	_subStates.wideRequestRandomize(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportChange(Control& control) {
	const UP	  h = _headState.deepReportChange(control);
	const Utility s = _subStates.wideReportChange(control);

	const Utility sub = s / WIDTH;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::UP
O_<TN, TA, TH, TS...>::deepReportUtilize(Control& control) {
	const UP	  h = _headState.deepReportUtilize(control);
	const Utility s = _subStates.wideReportUtilize(control);

	const Utility sub = s / WIDTH;

	HFSM_LOG_UTILITY_RESOLUTION(HEAD_ID, INVALID_STATE_ID, sub);

	return {
		h.utility * sub,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Rank
O_<TN, TA, TH, TS...>::deepReportRank(Control& control) {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
typename TA::Utility
O_<TN, TA, TH, TS...>::deepReportRandomize(Control& control) {
	const Utility h = _headState.wrapUtility(control);
	const Utility s = _subStates.wideReportRandomize(control);

	const Utility sub = s / WIDTH;

	HFSM_LOG_RANDOM_RESOLUTION(HEAD_ID, INVALID_STATE_ID, sub);

	return h * sub;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepEnterRequested(PlanControl& control) {
	_headState.deepEnter		 (control);
	_subStates.wideEnterRequested(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepChangeToRequested(PlanControl& control) {
	_subStates.wideChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, typename TH, typename... TS>
void
O_<TN, TA, TH, TS...>::deepGetNames(const LongIndex parent,
									const RegionType region,
									const ShortIndex depth,
									StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class R_ {
	using Config_				= TConfig;
	using Context				= typename Config_::Context;
	using Rank					= typename Config_::Rank;
	using Utility				= typename Config_::Utility;
	using Random_				= typename Config_::Random_;
	using Logger				= typename Config_::Logger;
	using Payload				= typename Config_::Payload;

	using Apex					= TApex;

	using ApexInfo				= Wrap<Apex>;
	using Info					= RF_<Config_, Apex>;
	using StateList				= typename Info::StateList;
	using RegionList			= typename Info::RegionList;

	static constexpr LongIndex SUBSTITUTION_LIMIT = Info::SUBSTITUTION_LIMIT;
	static constexpr LongIndex TASK_CAPACITY	  = Info::TASK_CAPACITY;

public:
	static constexpr LongIndex  REVERSE_DEPTH	  = ApexInfo::REVERSE_DEPTH;
	static constexpr ShortIndex COMPO_REGIONS	  = ApexInfo::COMPO_REGIONS;
	static constexpr LongIndex  COMPO_PRONGS	  = ApexInfo::COMPO_PRONGS;
	static constexpr ShortIndex ORTHO_REGIONS	  = ApexInfo::ORTHO_REGIONS;
	static constexpr ShortIndex ORTHO_UNITS		  = ApexInfo::ORTHO_UNITS;

	static constexpr LongIndex  STATE_COUNT		  = ApexInfo::STATE_COUNT;
	static constexpr LongIndex  REGION_COUNT	  = ApexInfo::REGION_COUNT;

	static_assert(STATE_COUNT <  (ShortIndex) -1, "Too many states in the hierarchy. Change 'ShortIndex' type.");
	static_assert(STATE_COUNT == (ShortIndex) StateList::SIZE, "STATE_COUNT != StateList::SIZE");

private:
	using Args					= typename Info::Args;

	using StateRegistry			= StateRegistryT<Args>;
	using AllForks				= typename StateRegistry::AllForks;

	using Control				= ControlT<Args>;

	using PlanControl			= PlanControlT<Args>;
	using PlanData				= PlanDataT   <Args>;

	using FullControl			= FullControlT<Args>;
	using Request				= typename FullControl::Request;
	using Requests				= typename FullControl::Requests;

	using GuardControl			= GuardControlT<Args>;

	using Payloads				= Array<Payload, STATE_COUNT>;
	using PayloadsSet			= BitArray<LongIndex, STATE_COUNT>;

	using MaterialApex			= Material<I_<0, 0, 0, 0>, Args, Apex>;

public:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos	= typename Args::StructureStateInfos;

	static constexpr LongIndex NAME_COUNT	  = MaterialApex::NAME_COUNT;

	using Prefix				= StaticArray<wchar_t, REVERSE_DEPTH * 2 + 2>;
	using Prefixes				= StaticArray<Prefix, STATE_COUNT>;

	using Structure				= Array<StructureEntry, NAME_COUNT>;
	using ActivityHistory		= Array<char,			NAME_COUNT>;

	using TransitionInfo		= TransitionInfoT<Payload>;
	using TransitionInfoStorage	= Array<TransitionInfo, COMPO_REGIONS * 4>;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	explicit R_(Context& context,
				Random_& random
				HFSM_IF_LOGGER(, Logger* const logger = nullptr));

	~R_();

	template <typename T>
	static constexpr StateID  stateId()					{ return			StateList ::template index<T>();	}

	template <typename T>
	static constexpr RegionID regionId()				{ return (RegionID) RegionList::template index<T>();	}

	void update();

	template <typename TEvent>
	HFSM_INLINE void react(const TEvent& event);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool isActive   (const StateID stateId) const	{ return _stateRegistry.isActive   (stateId);	}
	HFSM_INLINE bool isResumable(const StateID stateId) const	{ return _stateRegistry.isResumable(stateId);	}

	HFSM_INLINE bool isScheduled(const StateID stateId) const	{ return isResumable(stateId);					}

	template <typename TState>
	HFSM_INLINE bool isActive   () const						{ return isActive	(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isResumable() const						{ return isResumable(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE bool isScheduled() const						{ return isResumable<TState>();					}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void changeTo (const StateID stateId);
	HFSM_INLINE void changeTo (const StateID stateId, const Payload& payload);

	HFSM_INLINE void restart  (const StateID stateId);
	HFSM_INLINE void restart  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void resume	  (const StateID stateId);
	HFSM_INLINE void resume   (const StateID stateId, const Payload& payload);

	HFSM_INLINE void utilize  (const StateID stateId);
	HFSM_INLINE void utilize  (const StateID stateId, const Payload& payload);

	HFSM_INLINE void randomize(const StateID stateId);
	HFSM_INLINE void randomize(const StateID stateId, const Payload& payload);

	HFSM_INLINE void schedule (const StateID stateId);
	HFSM_INLINE void schedule (const StateID stateId, const Payload& payload);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TState>
	HFSM_INLINE void changeTo ()								{ changeTo (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void changeTo (const Payload& payload)			{ changeTo (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void restart  ()								{ restart  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void restart  (const Payload& payload)			{ restart  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void resume	  ()								{ resume   (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void resume	  (const Payload& payload)			{ resume   (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void utilize  ()								{ utilize  (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void utilize  (const Payload& payload)			{ utilize  (stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void randomize()								{ randomize(stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void randomize(const Payload& payload)			{ randomize(stateId<TState>(), payload);		}

	template <typename TState>
	HFSM_INLINE void schedule ()								{ schedule (stateId<TState>());					}

	template <typename TState>
	HFSM_INLINE void schedule (const Payload& payload)			{ schedule (stateId<TState>(), payload);		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void resetStateData(const StateID stateId);
	HFSM_INLINE void setStateData  (const StateID stateId, const Payload& payload);
	HFSM_INLINE bool isStateDataSet(const StateID stateId) const;

	HFSM_INLINE const Payload* getStateData(const StateID stateId) const;

	template <typename TState>
	HFSM_INLINE void resetStateData()							{ resetStateData(stateId<TState>());			}

	template <typename TState>
	HFSM_INLINE void setStateData  (const Payload& payload)		{ setStateData  (stateId<TState>(), payload);	}

	template <typename TState>
	HFSM_INLINE bool isStateDataSet() const						{ return isStateDataSet(stateId<TState>());		}

	template <typename TState>
	HFSM_INLINE const Payload* getStateData() const				{ return getStateData(stateId<TState>());		}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	const Structure&	   structure()		 const				{ return _structure;							}
	const ActivityHistory& activityHistory() const				{ return _activityHistory;						}
#endif

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_ENABLE_VERBOSE_DEBUG_LOG
	void attachLogger(Logger* const logger)						{ _logger = logger;								}
#endif

private:
	void initialEnter();
	void processTransitions();

	bool applyRequests(Control& control);

	bool cancelledByEntryGuards(const Requests& pendingChanges);
	bool cancelledByGuards(const Requests& pendingChanges);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	void getStateNames();
	void udpateActivity();
	void recordRequestsAs(const Method method);
#endif

private:
	Context& _context;
	Random_& _random;

	StateRegistry _stateRegistry;
	PlanData _planData;

	Payloads _payloads;
	PayloadsSet _payloadsSet;

	Requests _requests;

	MaterialApex _apex;

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	Prefixes _prefixes;
	StructureStateInfos _stateInfos;

	Structure _structure;
	ActivityHistory _activityHistory;

	TransitionInfoStorage _lastTransitions;
#endif

	HFSM_IF_LOGGER(Logger* _logger);
};

////////////////////////////////////////////////////////////////////////////////

template <typename TConfig,
		  typename TApex>
class RW_ final
	: public R_<TConfig, TApex>
{
public:
	using R_<TConfig, TApex>::R_;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN,
		  typename TU,
		  typename TR,
		  typename TP,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, TR, TP, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, TR, TP, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, TR, TP, NS, NT>;
	using Context	= typename Config_::Context;
	using Random_	= typename Config_::Random_;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(Random_& random
							 HFSM_IF_LOGGER(, Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			random
			HFSM_IF_LOGGER(, logger)}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC,
		  typename TN,
		  typename TU,
		  typename TP,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<TC, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<TC, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>, TApex>
	, ::hfsm2::RandomT<TU>
{
	using Config_	= ::hfsm2::ConfigT<TC, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>;
	using Context	= typename Config_::Context;
	using Random_	= typename Config_::Random_;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(Context& context
							 HFSM_IF_LOGGER(, Logger* const logger = nullptr))
		: R{context,
			static_cast<Random_&>(*this)
			HFSM_IF_LOGGER(, logger)}
		, Random_{0}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN,
		  typename TU,
		  typename TP,
		  LongIndex NS,
		  LongIndex NT,
		  typename TApex>
class RW_	   <::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>, TApex> final
	: public R_<::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>, TApex>
	, ::hfsm2::EmptyContext
	, ::hfsm2::RandomT<TU>
{
	using Config_	= ::hfsm2::ConfigT<::hfsm2::EmptyContext, TN, TU, ::hfsm2::RandomT<TU>, TP, NS, NT>;
	using Context	= typename Config_::Context;
	using Random_	= typename Config_::Random_;
	using Logger	= typename Config_::Logger;

	using R			= R_<Config_, TApex>;

public:
	explicit HFSM_INLINE RW_(HFSM_IF_LOGGER(Logger* const logger = nullptr))
		: R{static_cast<Context&>(*this),
			static_cast<Random_&>(*this)
			HFSM_IF_LOGGER(, logger)}
		, Random_{0}
	{}
};

////////////////////////////////////////////////////////////////////////////////

}
}

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
R_<TG, TA>::R_(Context& context,
			   Random_& random
			   HFSM_IF_LOGGER(, Logger* const logger))
	: _context{context}
	, _random{random}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() {
	PlanControl control{_context,
						_random,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::update() {
	FullControl control(_context,
						_random,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepUpdate(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
void
R_<TG, TA>::react(const TEvent& event) {
	FullControl control(_context,
						_random,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(control, event);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId) {
	const Request request{Request::Type::CHANGE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId,
					 const Payload& payload)
{
	const Request request{Request::Type::CHANGE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::CHANGE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::restart(const StateID stateId) {
	const Request request{Request::Type::RESTART, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::restart(const StateID stateId,
					const Payload& payload)
{
	const Request request{Request::Type::RESTART, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESTART, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::resume(const StateID stateId) {
	const Request request{Request::Type::RESUME, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::resume(const StateID stateId,
				   const Payload& payload)
{
	const Request request{Request::Type::RESUME, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESUME, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::utilize(const StateID stateId) {
	const Request request{Request::Type::UTILIZE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::utilize(const StateID stateId,
					const Payload& payload)
{
	const Request request{Request::Type::UTILIZE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::UTILIZE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::randomize(const StateID stateId) {
	const Request request{Request::Type::RANDOMIZE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RANDOMIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::randomize(const StateID stateId,
					  const Payload& payload)
{
	const Request request{Request::Type::RANDOMIZE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RANDOMIZE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::schedule(const StateID stateId) {
	const Request request{Request::Type::SCHEDULE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::schedule(const StateID stateId,
					 const Payload& payload)
{
	const Request request{Request::Type::SCHEDULE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::resetStateData(const StateID stateId) {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_payloadsSet.reset(stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::setStateData(const StateID stateId,
						 const Payload& payload)
{
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		_payloads[stateId] = payload;
		_payloadsSet.set(stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::isStateDataSet(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	return stateId < Payloads::CAPACITY ?
		_payloadsSet.get(stateId) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
const typename R_<TG, TA>::Payload*
R_<TG, TA>::getStateData(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	return stateId < Payloads::CAPACITY && _payloadsSet.get(stateId) ?
		&_payloads[stateId] : nullptr;
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::initialEnter() {
	Control control(_context,
					_random,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr));

	AllForks undoRequested = _stateRegistry.requested;

	_apex.deepRequestChange(control);

	Requests lastRequests = _requests;
	_requests.clear();

	if (cancelledByEntryGuards(lastRequests))
		_stateRegistry.requested = undoRequested;

	for (LongIndex i = 0;
		 i < SUBSTITUTION_LIMIT && _requests.count();
		 ++i)
	{
		undoRequested = _stateRegistry.requested;

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByEntryGuards(lastRequests))
				_stateRegistry.requested = undoRequested;
		}

		_requests.clear();
	}
	HFSM_ASSERT(_requests.count() == 0);

	{
		PlanControl planControl{_context,
								_random,
								_stateRegistry,
								_planData,
								HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepEnterRequested(planControl);
		_stateRegistry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	AllForks undoRequested;
	Requests lastRequests;

	Control control(_context,
					_random,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr));

	for (LongIndex i = 0;
		i < SUBSTITUTION_LIMIT && _requests.count();
		++i)
	{
		undoRequested = _stateRegistry.requested;

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByGuards(lastRequests))
				_stateRegistry.requested = undoRequested;
		} else
			_requests.clear();
	}

	{
		PlanControl planControl{_context,
								_random,
								_stateRegistry,
								_planData,
								HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepChangeToRequested(planControl);
		_stateRegistry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequests(Control& control) {
	bool changesMade = false;

	for (const Request& request : _requests) {
		HFSM_IF_STRUCTURE(_lastTransitions << TransitionInfo(request, Method::UPDATE));

		switch (request.type) {
		case Request::CHANGE:
		case Request::RESTART:
		case Request::RESUME:
		case Request::UTILIZE:
		case Request::RANDOMIZE:
			if (_stateRegistry.requestImmediate(request))
				_apex.deepForwardActive(control, request.type);
			else
				_apex.deepRequest	   (control, request.type);

			changesMade = true;
			break;

		case Request::SCHEDULE:
			_stateRegistry.requestScheduled(request.stateId);
			break;

		default:
			HFSM_BREAK();
		}
	}

	return changesMade;
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByEntryGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context,
							  _random,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
		HFSM_LOGGER_OR(_logger, nullptr)};

	if (_apex.deepEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context,
							  _random,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
							  HFSM_LOGGER_OR(_logger, nullptr)};

	if (_apex.deepForwardExitGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::EXIT_GUARD));

		return true;
	} else if (_apex.deepForwardEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TG, typename TA>
void
R_<TG, TA>::getStateNames() {
	_stateInfos.clear();
	_apex.deepGetNames((LongIndex) -1, StructureStateInfo::COMPOSITE, 0, _stateInfos);

	LongIndex margin = (LongIndex) -1;
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const LongIndex mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.region == StructureStateInfo::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (auto d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (auto r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.region == StructureStateInfo::COMPOSITE ? L'│' : L'║';
					break;
				case L'└':
					prefixAbove[mark] = L'├';
					break;
				case L'╙':
					prefixAbove[mark] = L'╟';
					break;
				}
			}
		}
	}
	if (margin > 0)
		margin -= 1;

	_structure.clear();
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const LongIndex space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure << StructureEntry { false, &prefix[margin * 2], state.name };
			_activityHistory << (char) 0;
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (LongIndex c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const LongIndex mark = space + 1;

			switch (nextPrefix[mark]) {
			case L'├':
				nextPrefix[mark] = state.depth == margin ? L'┌' : L'┬';
				break;
			case L'╟':
				nextPrefix[mark] = state.depth == margin ? L'╓' : L'╥';
				break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::udpateActivity() {
	for (LongIndex s = 0, i = 0; s < _stateInfos.count(); ++s)
		if (_stateInfos[s].name[0] != L'\0') {
			_structure[i].isActive = isActive(s);

			auto& activity = _activityHistory[i];

			if (_structure[i].isActive) {
				if (activity < 0)
					activity = +1;
				else
					activity = activity < INT8_MAX ? activity + 1 : activity;
			} else {
				if (activity > 0)
					activity = -1;
				else
					activity = activity > INT8_MIN ? activity - 1 : activity;
			}

			++i;
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_lastTransitions << TransitionInfo(request, method);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#undef HFSM_INLINE
#undef HFSM_IF_LOGGER
#undef HFSM_LOGGER_OR
#undef HFSM_LOG_STATE_METHOD
#undef HFSM_IF_STRUCTURE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
