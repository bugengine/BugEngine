/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/marshallers/variant.hh>
#include    <rtti/object.hh>

namespace BugEngine { namespace RTTI
{

ValueTypeIndex Marshaller<Value>::type() const
{
    return PropertyTypeVariant;
}

ValueCopy Marshaller<Value>::get(void* source) const
{
    UNUSED(source);
    throw 0;
}

ValueCopy Marshaller<Value>::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

ValueCopy Marshaller<Value>::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

void Marshaller<Value>::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

void Marshaller<Value>::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

void Marshaller<Value>::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

}}
