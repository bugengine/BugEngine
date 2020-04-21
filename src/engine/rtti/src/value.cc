/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/value.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/typeinfo.inl>

namespace BugEngine { namespace RTTI
{

Value::Value(const Value& other)
:   m_type(other.m_type)
,   m_reference(other.m_reference)
{
    if (m_reference)
    {
        m_ref.m_pointer = other.m_ref.m_pointer;
        m_ref.m_deallocate = false;
    }
    else
    {
        m_ref.m_pointer = m_type.size() > sizeof(m_buffer)
                 ? Arena::script().alloc(m_type.size())
                 : 0;
        m_ref.m_deallocate = (m_ref.m_pointer != 0);
        m_type.copy(other.memory(), memory());
    }
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
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? Arena::script().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
}

Value::~Value()
{
    if (!m_reference)
    {
        m_type.destroy(memory());
        if (m_type.size() > sizeof(m_buffer) && m_ref.m_deallocate)
        {
            Arena::script().free(m_ref.m_pointer);
        }
    }
}

Value& Value::operator=(const Value& v)
{
    if (m_reference)
    {
        be_assert_recover(m_type.isA(v.m_type),
                          "Value has type %s; unable to copy from type %s"
                          | m_type | v.m_type, return *this);
        be_assert_recover(m_type.constness != Type::Const, "Value is const", return *this);
        void* mem = memory();
        m_type.destroy(mem);
        m_type.copy(v.memory(), mem);
    }
    else
    {
        this->~Value();
        new ((void*)this) Value(v);
    }
    return *this;
}

void* Value::unpackAs(const Type& ti,
                      ref<minitl::refcountable>& rptr,
                      weak<minitl::refcountable>& wptr,
                      minitl::refcountable*& obj)
{
    be_assert(m_type.isA(ti), "Value has type %s; unable to unbox to type %s" | m_type | ti);
    void* mem = memory();
    switch(m_type.indirection)
    {
    case Type::RefPtr:
        if (ti.indirection == Type::RefPtr)
            break;
        rptr = *(ref<minitl::refcountable>*)mem;
        wptr = rptr;
        mem = (void*)&wptr;
        /* falls through */
    case Type::WeakPtr:
        if (ti.indirection == Type::WeakPtr)
            break;
        wptr = *(weak<minitl::refcountable>*)mem;
        obj = wptr.operator->();
        mem = (void*)&obj;
        /* falls through */
    case Type::RawPtr:
        if (ti.indirection == Type::RawPtr)
            break;
        mem = *(void**)mem;
        /* falls through */
    default:
        break;
    }
    return mem;
}

void Value::store(const void* src)
{
    m_ref.m_pointer = m_type.size() > sizeof(m_buffer) ? Arena::script().alloc(m_type.size()) : 0;
    m_ref.m_deallocate = m_ref.m_pointer != 0;
    m_type.copy(src, memory());
}

Value Value::operator[](const istring& name)
{
    bool found;
    return m_type.metaclass->get(*this, name, found);
}

Value Value::operator[](const istring& name) const
{
    bool found;
    return m_type.metaclass->get(*this, name, found);
}

Value Value::operator()(Value params[], u32 paramCount)
{
    static const istring callName("?call");
    Value call = (*this)[callName];
    be_assert_recover(call,
                      "Not a callable object: %s" | m_type,
                      return Value());
    be_assert_recover(call.isA(be_type<const Method* const>()),
                      "Not a callable object: %s" | m_type,
                      return Value());
    return call.as<const Method* const>()->doCall(params, paramCount);
}

void Value::swap(Value& other)
{
    if (&other != this)
    {
        if (m_reference && other.m_reference)
        {
            minitl::swap(m_type, other.m_type);
            minitl::swap(m_ref, other.m_ref);
        }
        else if (m_reference)
        {
            minitl::swap(m_type, other.m_type);
            minitl::swap(m_reference, other.m_reference);
            Reference r = m_ref;
            m_type.copy(other.m_buffer, m_buffer);
            m_type.destroy(other.m_buffer);
            other.m_ref = r;
        }
        else if (other.m_reference)
        {
            Reference r = other.m_ref;
            m_type.copy(m_buffer, other.m_buffer);
            m_type.destroy(m_buffer);
            m_ref = r;
            minitl::swap(m_type, other.m_type);
            minitl::swap(m_reference, other.m_reference);
        }
        else
        {
            Value v(*this);
            *this = other;
            other = v;
        }
    }
}

}}
