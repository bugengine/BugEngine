/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/typeinfo.inl>

namespace BugEngine
{

Value::Value()
:   m_type(be_typeid<void>::type())
,   m_reference(false)
{
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

Value::Value(TypeInfo type, void* location)
:   m_type(type)
,   m_reference(true)
{
    m_ref.m_pointer = location;
    m_ref.m_deallocate = false;
}

Value::Value(TypeInfo type, ReserveType)
:   m_type(type)
,   m_reference(false)
{
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? scriptArena().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
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
        be_assert_recover(v.m_type.metaclass == m_type.metaclass, "Value has type %s; unable to copy from type %s" | m_type.name() | v.m_type.name(), return *this);
        be_assert_recover(m_type.constness != TypeInfo::Const, "Value is const", return *this);
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

}
