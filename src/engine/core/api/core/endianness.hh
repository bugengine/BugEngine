/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/


#ifndef BE_CORE_ENDIANNESS_HH_
#define BE_CORE_ENDIANNESS_HH_
/*****************************************************************************/

namespace BugEngine
{

enum Endianness
{
    Endianness_Little,
    Endianness_Big,
    Endianness_Self =
#ifndef BE_BIGENDIAN
        Endianness_Little
#else
        Endianness_Big
#endif
};


template< typename T, Endianness from, Endianness to >
struct ByteSwap;

template< typename T >
struct ByteSwap<T, Endianness_Little, Endianness_Little>
{
    static inline T byteswap(T value)
    {
        return value;
    }
};

template< typename T >
struct ByteSwap<T, Endianness_Big, Endianness_Big>
{
    static inline T byteswap(T value)
    {
        return value;
    }
};

template< typename T >
struct ByteSwap<T, Endianness_Big, Endianness_Little>
{
    static inline T byteswap(T value)
    {
        T result = 0;
        for (unsigned i = 0; i < sizeof(T)/2; ++i)
        {
            result = (value & (0xff << i)) << (8*((sizeof(T))-i*2-1));
        }
        for (unsigned i = sizeof(T)/2; i < sizeof(T); ++i)
        {
            result = (value & (0xff << i)) >> (8*(i*2+1));
        }

        return result;
    }
};

template< typename T >
struct ByteSwap<T, Endianness_Little, Endianness_Big>
{
    static inline T byteswap(T value)
    {
        return ByteSwap<T, Endianness_Big, Endianness_Little>::byteswap(value);
    }
};

template< typename T, Endianness e >
class Integer
{
private:
    T   m_value;
public:
    Integer(T value = 0) : m_value(ByteSwap<T, Endianness_Self, e>::byteswap(m_value)) { }
    ~Integer()                                                       { }
    operator T() const                                               { return ByteSwap<T, e, Endianness_Self>::byteswap(m_value); }
    Integer& operator=(T value)                                      { m_value = ByteSwap<T, Endianness_Self, e>::byteswap(m_value); }
};

typedef Integer<u8, Endianness_Little>  u8_l;
typedef Integer<u16, Endianness_Little> u16_l;
typedef Integer<u32, Endianness_Little> u32_l;
typedef Integer<u64, Endianness_Little> u64_l;
typedef Integer<i8, Endianness_Little>  i8_l;
typedef Integer<i16, Endianness_Little> i16_l;
typedef Integer<i32, Endianness_Little> i32_l;
typedef Integer<i64, Endianness_Little> i64_l;

typedef Integer<u8, Endianness_Big>     u8_b;
typedef Integer<u16, Endianness_Big>    u16_b;
typedef Integer<u32, Endianness_Big>    u32_b;
typedef Integer<u64, Endianness_Big>    u64_b;
typedef Integer<i8, Endianness_Big>     i8_b;
typedef Integer<i16, Endianness_Big>    i16_b;
typedef Integer<i32, Endianness_Big>    i32_b;
typedef Integer<i64, Endianness_Big>    i64_b;
}

/*****************************************************************************/
#endif

