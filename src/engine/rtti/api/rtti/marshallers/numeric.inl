/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NUMERIC_MARSHALLER_INL_
#define BE_RTTI_NUMERIC_MARSHALLER_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename FROM, typename TO >
ValueTypeIndex NumericMarshaller<FROM, TO>::type() const
{
    return ValueTypeIndex(minitl::indexof< TO, ValueTypes >::Value);
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source) const
{
    return castfrom(*reinterpret_cast<FROM*>(source));
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source, size_t index) const
{
    UNUSED(source);
    UNUSED(index);
    throw 0;
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::get(void* source, const Value& key) const
{
    UNUSED(source);
    UNUSED(key);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst) const
{
    *reinterpret_cast<FROM*>(dst) = castto(value);
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst, size_t index) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(index);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::set(const Value& value, void* dst, const Value& key) const
{
    UNUSED(value);
    UNUSED(dst);
    UNUSED(key);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::push_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::push_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::pop_back(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
void NumericMarshaller<FROM, TO>::pop_front(const Value& value, void* dst) const
{
    UNUSED(value);
    UNUSED(dst);
    throw 0;
}

template< typename FROM, typename TO >
ValueCopy NumericMarshaller<FROM, TO>::castfrom(FROM value) const
{
    return ValueCopy(be_checked_numcast<TO>(value));
}

template< typename FROM, typename TO >
FROM NumericMarshaller<FROM, TO>::castto(const Value& value) const
{
    switch(value.type())
    {
    case RTTI::PropertyTypeFloat:
        return be_checked_numcast<FROM>(value.member<double>());
    case RTTI::PropertyTypeUnsigned:
        return be_checked_numcast<FROM>(value.member<u64>());
    case RTTI::PropertyTypeInteger:
        return be_checked_numcast<FROM>(value.member<i64>());
    default:
        be_notreached();
        return 0;
    }
}

}}


/*****************************************************************************/
#endif
