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
    m_ref.m_pointer = other.m_reference ? other.m_ref.m_pointer : (m_type.size() > sizeof(m_buffer) ? Arena::script().alloc(m_type.size()) : 0);
    m_ref.m_deallocate = other.m_reference ? false : (m_ref.m_pointer != 0);
    if (!m_reference)
    {
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
        be_assert_recover(m_type.isA(v.m_type), "Value has type %s; unable to copy from type %s" | m_type | v.m_type, return *this);
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

void* Value::unpackAs(const Type& ti, ref<minitl::refcountable>& rptr, weak<minitl::refcountable>& wptr, minitl::refcountable*& obj)
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

Value Value::operator()(Value params[], u32 paramCount)
{
    static const istring callName("?call");
    Value call = (*this)[callName];
    be_assert_recover(call, "Not a callable object: %s" | m_type, return Value());
    be_assert_recover(call.isA(be_typeid<const Method* const>::type()), "Not a callable object: %s" | m_type, return Value());
    return call.as<const Method* const>()->doCall(params, paramCount);
}

}}
