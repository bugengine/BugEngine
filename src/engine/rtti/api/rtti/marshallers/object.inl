/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_OBJECT_MARSHALLER_INL_
#define BE_RTTI_OBJECT_MARSHALLER_INL_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
ValueTypeIndex Marshaller< refptr<T> >::type() const
{
    return ValueTypeIndex(minitl::indexof< refptr<Object>, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source) const
{
    return castfrom(*reinterpret_cast<refptr<T>*>(source));
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst) const
{
    *reinterpret_cast<refptr<T>*>(dst) = castto(value);
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
void Marshaller< refptr<T> >::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename T >
ValueCopy Marshaller< refptr<T> >::castfrom(refptr<T> value) const
{
    return ValueCopy(refptr<Object>(value.get()));
}

template< typename T >
refptr<T> Marshaller< refptr<T> >::castto(const Value& value) const
{
    return refptr<T>(checked_cast<T*>(value.member< refptr<Object> >().get()));
}

//-----------------------------------------------------------------------------

template< typename T >
ValueTypeIndex Marshaller<T*>::type() const
{
    return ValueTypeIndex(minitl::indexof< Object*, ValueTypes >::Value);
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* source) const
{
    return castfrom(*reinterpret_cast<T**>(source));
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* /*source*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller<T*>::get(void* /*source*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<T**>(dst) = castto(value);
}

template< typename T >
void Marshaller<T*>::set(const Value& /*value*/, void* /*dst*/, size_t /*index*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::set(const Value& /*value*/, void* /*dst*/, const Value& /*key*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::push_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::push_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::pop_back(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
void Marshaller<T*>::pop_front(const Value& /*value*/, void* /*dst*/) const
{
    throw 0;
}

template< typename T >
ValueCopy Marshaller<T*>::castfrom(T* value) const
{
    return ValueCopy(const_cast<Object*>(static_cast<const Object*>(value)));
}

template< typename T >
T* Marshaller<T*>::castto(const Value& value) const
{
    return checked_cast<T*>(value.member<Object*>());
}


}}


/*****************************************************************************/
#endif
