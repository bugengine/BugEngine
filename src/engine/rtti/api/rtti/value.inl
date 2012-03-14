/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_INL_
#define BE_RTTI_VALUE_INL_
/*****************************************************************************/
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <minitl/type/typemanipulation.hh>
#include    <rtti/typeinfo.inl>

namespace BugEngine
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
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? scriptArena().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
    be_assert(be_typeid<T>::type() <= m_type, "specific typeinfo %s and typeid %s are not compatible" | m_type.name() | be_typeid<T>::type().name());
    m_type.copy(&t, memory());
}

template< typename T >
Value::Value(T t, MakeConstType /*constify*/)
:   m_type(be_typeid<T>::type(), Type::MakeConst)
,   m_reference(false)
{
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? scriptArena().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
    be_assert(be_typeid<T>::type() <= m_type, "specific typeinfo %s and typeid %s are not compatible" | m_type.name() | be_typeid<T>::type().name());
    m_type.copy(&t, memory());
}

Value::Value(const Value& other)
:   m_type(other.m_type)
,   m_reference(other.m_reference)
{
    m_ref.m_pointer = other.m_reference ? other.m_ref.m_pointer : (m_type.size() > sizeof(m_buffer) ? scriptArena().alloc(m_type.size()) : 0);
    m_ref.m_deallocate = other.m_reference ? false : (m_ref.m_pointer != 0);
    if (!m_reference)
        m_type.copy(other.memory(), memory());
}

Value::Value(Type type, void* location)
:   m_type(type)
,   m_reference(true)
{
    m_ref.m_pointer = location;
    m_ref.m_deallocate = false;
}

Value::Value(Type type, ReserveType)
:   m_type(type)
,   m_reference(false)
{
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? scriptArena().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
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

Value::~Value()
{
    if (!m_reference)
    {
        m_type.destroy(memory());
        if (m_type.size() > sizeof(m_buffer) && m_ref.m_deallocate)
        {
            scriptArena().free(m_ref.m_pointer);
        }
    }
}

Value& Value::operator=(const Value& v)
{
    if (m_reference)
    {
        be_assert_recover(v.m_type <= m_type, "Value has type %s; unable to copy from type %s" | m_type.name() | v.m_type.name(), return *this);
        be_assert_recover(m_type.constness != Type::Const, "Value is const", return *this);
        void* mem = memory();
        m_type.destroy(mem);
        m_type.copy(v.memory(), mem);
        return *this;
    }
    else
    {
        this->~Value();
        new ((void*)this) Value(v);
        return *this;
    }
}

template< typename T >
Value& Value::operator=(const T& t)
{
    if (m_reference)
    {
        be_assert_recover(be_typeid<T>::type() <= m_type, "Value has type %s; unable to copy from type %s" | m_type.name() | be_typeid<T>::type().name(), return *this);
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
    return const_cast<Value*>(this)->as<const T>();
}

template< typename T >
T Value::as()
{
    typedef typename minitl::remove_reference<T>::type REALTYPE;
    Type ti = be_typeid<T>::type();
    be_assert(m_type.metaclass->isA(ti.metaclass), "Value has type %s; unable to unbox to type %s" | m_type.name() | ti.name());
    be_assert(ti.indirection <= m_type.indirection, "Value has type %s; unable to unbox to type %s" | m_type.name() | ti.name());
    be_assert(ti.access <= m_type.access, "Value has type %s; unable to unbox to type %s" | m_type.name() | ti.name());
    void* mem = memory();
    ref<minitl::refcountable>   rptr;
    weak<minitl::refcountable>  wptr;
    minitl::refcountable*       obj;
    switch(m_type.indirection)
    {
    case Type::RefPtr:
        if (ti.indirection == Type::RefPtr)
            break;
        rptr = *(ref<minitl::refcountable>*)mem;
        wptr = rptr;
        mem = (void*)&wptr;
    case Type::WeakPtr:
        if (ti.indirection == Type::WeakPtr)
            break;
        wptr = *(weak<minitl::refcountable>*)mem;
        obj = wptr.operator->();
        mem = (void*)&obj;
    case Type::RawPtr:
        if (ti.indirection == Type::RawPtr)
            break;
        mem = *(void**)mem;
    default:
        break;
    }
    return *(REALTYPE*)mem;
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

Value Value::operator[](const istring& name)
{
    return m_type.metaclass->get(*this, name);
}

Value Value::operator()(Value params[], u32 paramCount)
{
    static const istring callName("call");
    Value call = (*this)[callName];
    be_assert_recover(call, "Not a callable object: %s" | m_type.name(), return Value());
    be_assert_recover(be_typeid<const RTTI::Method*>::type() <= call.type(), "Not a callable object: %s" | m_type.name(), return Value());
    return call.as<const RTTI::Method*>()->doCall(params, paramCount);
}

}

#include    <rtti/typeinfo.inl>

/*****************************************************************************/
#endif
