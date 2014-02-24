/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/**************************************************************************************************/
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <minitl/typemanipulation.hh>

namespace BugEngine { namespace RTTI
{

Value::Value()
:   m_type(be_typeid<void>::type())
,   m_reference(false)
{
}

template< typename T >
Value::Value(T t)
:   m_type(be_typeid<T>::type())
,   m_reference(false)
{
    store(&t);
}

template< typename T >
Value::Value(T t, MakeConstType /*constify*/)
:   m_type(Type::makeType(be_typeid<T>::type(), Type::MakeConst))
,   m_reference(false)
{
    store(&t);
}

template< typename T >
Value::Value(ByRefType<T> t)
:   m_type(be_typeid<T>::type())
,   m_reference(true)
{
    m_ref.m_pointer = const_cast<void*>((const void*)&t.value);
    m_ref.m_deallocate = false;
}

template<>
inline Value::Value(ByRefType<Value> t)
:   m_type(t.value.m_type)
,   m_reference(true)
{
    m_ref.m_pointer = t.value.memory();
    m_ref.m_deallocate = false;
}

template<>
inline Value::Value(ByRefType<const Value> t)
:   m_type(Type::makeType(t.value.m_type, Type::MakeConst))
,   m_reference(true)
{
    m_ref.m_pointer = const_cast<void*>(t.value.memory());
    m_ref.m_deallocate = false;
}

template< typename T >
Value& Value::operator=(const T& t)
{
    if (m_reference)
    {
        be_assert_recover(m_type.isA(be_typeid<T>::type()), "Value has type %s; unable to copy from type %s" | m_type | be_typeid<T>::type(), return *this);
        be_assert_recover(m_type.constness != Type::Const, "Value is const", return *this);
        void* mem = memory();
        m_type.destroy(mem);
        m_type.copy(&t, mem);
        return *this;
    }
    else
    {
        this->~Value();
        new ((void*)this) Value(t);
        return *this;
    }
}

Type Value::type()
{
    return m_type;
}

Type Value::type() const
{
    return Type::makeType(m_type, Type::MakeConst);
}

template< typename T >
const T Value::as() const
{
    return const_cast<Value*>(this)->as<T>();
}

template< typename T >
typename minitl::remove_const<T>::type Value::as()
{
    typedef typename minitl::remove_reference<T>::type REALTYPE;
    Type ti = be_typeid<T>::type();
    ref<minitl::refcountable>   rptr;
    weak<minitl::refcountable>  wptr;
    minitl::refcountable*       obj;
    return *(REALTYPE*)unpackAs(ti, rptr, wptr, obj);
}

template< >
inline Value& Value::as<Value&>()
{
    return *this;
}

template< >
inline const Value& Value::as<const Value&>()
{
    return *this;
}

void* Value::memory()
{
    if (!m_reference && m_type.size() <= sizeof(m_buffer))
    {
        return m_buffer;
    }
    else
    {
        return m_ref.m_pointer;
    }
}

const void* Value::memory() const
{
    if (!m_reference && m_type.size() <= sizeof(m_buffer))
    {
        return m_buffer;
    }
    else
    {
        return m_ref.m_pointer;
    }
}

bool Value::isConst() const
{
    return m_type.access == Type::Const;
}

Value::operator const void*() const
{
    return (const void*)(m_type.metaclass != be_typeid<void>::klass());
}

bool Value::operator!() const
{
    return m_type.metaclass == be_typeid<void>::klass();
}

void* Value::rawget() const
{
    return m_type.rawget(memory());
}

}}

#include    <rtti/typeinfo.inl>

/**************************************************************************************************/
#endif
