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

#ifndef BE_RTTI_NUMERIC_MARSHALLER_HH_
#define BE_RTTI_NUMERIC_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/marshallers/abstract.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename FROM, typename TO >
class NumericMarshaller : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy get(void* source)                                     const override;
    virtual ValueCopy get(void* source, size_t index)                       const override;
    virtual ValueCopy get(void* source, const Value& key)                   const override;
    virtual void      set(const Value& value, void* dst)                    const override;
    virtual void      set(const Value& value, void* dst, size_t index)      const override;
    virtual void      set(const Value& value, void* dst, const Value& key)  const override;
    virtual void      push_back(const Value& value, void* dst)              const override;
    virtual void      push_front(const Value& value, void* dst)             const override;
    virtual void      pop_back(const Value& value, void* dst)               const override;
    virtual void      pop_front(const Value& value, void* dst)              const override;

    inline ValueCopy  castfrom(FROM value)                                  const;
    inline FROM       castto(const Value& value)                            const;
};

template< >
class Marshaller<bool> : public NumericMarshaller<bool,bool>
{
};

template< >
class Marshaller<i8> : public NumericMarshaller<i8,i64>
{
};

template< >
class Marshaller<i16> : public NumericMarshaller<i16,i64>
{
};

template< >
class Marshaller<i32> : public NumericMarshaller<i32,i64>
{
};

template< >
class Marshaller<i64> : public NumericMarshaller<i64,i64>
{
};

template< >
class Marshaller<u8> : public NumericMarshaller<u8,u64>
{
};

template< >
class Marshaller<u16> : public NumericMarshaller<u16,u64>
{
};

template< >
class Marshaller<u32> : public NumericMarshaller<u32,u64>
{
};

template< >
class Marshaller<u64> : public NumericMarshaller<u64,u64>
{
};

template< >
class Marshaller<float> : public NumericMarshaller<float,double>
{
};

template< >
class Marshaller<double> : public NumericMarshaller<double,double>
{
};

}}

#include    <rtti/marshallers/numeric.inl>

/*****************************************************************************/
#endif
