/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_OBJECT_MARSHALLER_INL_
#define BE_RTTI_OBJECT_MARSHALLER_INL_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
ValueTypeIndex Marshaller< ref<T> >::type() const
{
    return ValueTypeIndex(minitl::indexof< ref<Object>, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller< ref<T> >::get(void* source) const
{
    return castfrom(*reinterpret_cast<ref<T>*>(source));
}

template< typename T >
ValueCopy Marshaller< ref<T> >::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< ref<T> >::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::set(const Value& value, void* dst) const
{
    *reinterpret_cast<ref<T>*>(dst) = castto(value);
}

template< typename T >
void Marshaller< ref<T> >::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< ref<T> >::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< ref<T> >::castfrom(ref<T> value) const
{
    return ValueCopy(be_const_cast<Object>(ref<const Object>(value)));
}

template< typename T >
ref<T> Marshaller< ref<T> >::castto(const Value& value) const
{
    return be_checked_cast<T>(value.member< ref<Object> >());
}

//-----------------------------------------------------------------------------

template< typename T >
ValueTypeIndex Marshaller< weak<T> >::type() const
{
    return ValueTypeIndex(minitl::indexof< weak<Object>, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller< weak<T> >::get(void* source) const
{
    return castfrom(*reinterpret_cast< weak<T> *>(source));
}

template< typename T >
ValueCopy Marshaller< weak<T> >::get(void* /*source*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller< weak<T> >::get(void* /*source*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::set(const Value& value, void* dst) const
{
    *reinterpret_cast< weak<T> *>(dst) = castto(value);
}

template< typename T >
void Marshaller< weak<T> >::set(const Value& /*value*/, void* /*dst*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::set(const Value& /*value*/, void* /*dst*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::push_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::push_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::pop_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller< weak<T> >::pop_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller< weak<T> >::castfrom(weak<T> value) const
{
    return ValueCopy(be_const_cast< Object >(weak<const Object>(value)));
}

template< typename T >
weak<T>  Marshaller< weak<T> >::castto(const Value& value) const
{
    return be_checked_cast<T>(value.member< weak<Object> >());
}


}}


/*****************************************************************************/
#endif
