/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>
#include   <rtti/engine/propertyinfo.script.hh>

namespace BugEngine
{

u32 TypeInfo::size() const
{
    switch(type)
    {
    case Class:
        return metaclass->size;
    case RawPtr:
    case ConstRawPtr:
        return sizeof(raw<char>);
    case RefPtr:
    case ConstRefPtr:
        return sizeof(ref<minitl::refcountable>);
    case WeakPtr:
    case ConstWeakPtr:
        return sizeof(weak<minitl::refcountable>);
    default:
        be_notreached();
        return 0;
    }
}

minitl::format<> TypeInfo::name() const
{
    minitl::format<> n("");
    switch(constness)
    {
    case Const:
        n = minitl::format<>("const %s") | metaclass->name.m_ptr;
        break;
    case Mutable:
        n = minitl::format<>(metaclass->name.m_ptr);
        break;
    default:
        be_notreached();
        return "";
    }
    switch(type)
    {
    case Class:
        return n;
    case RawPtr:
        return minitl::format<>("raw<%s>") | n;
    case ConstRawPtr:
        return minitl::format<>("raw<const %s>") | n;
    case RefPtr:
        return minitl::format<>("ref<%s>") | n;
    case ConstRefPtr:
        return minitl::format<>("ref<const %s>") | n;
    case WeakPtr:
        return minitl::format<>("weak<%s>") | n;
    case ConstWeakPtr:
        return minitl::format<>("weak<const %s>") | n;
    default:
        be_notreached();
        return "";
    }
}

void TypeInfo::copy(const void* source, void* dest) const
{
    switch(type)
    {
    case Class:
        return metaclass->copy(source, dest);
    case RawPtr:
    case ConstRawPtr:
        memcpy(dest, source, sizeof(void*));
        return;
    case RefPtr:
    case ConstRefPtr:
        new (dest) const ref<const minitl::refcountable>(*(const ref<const minitl::refcountable>*)source);
        return;
    case WeakPtr:
    case ConstWeakPtr:
        new (dest) const weak<const minitl::refcountable>(*(const weak<const minitl::refcountable>*)source);
        return;
    default:
        be_notreached();
        return;
    }
}

void TypeInfo::destroy(void* ptr) const
{
    switch(type)
    {
    case Class:
        return metaclass->destroy(ptr);
    case RawPtr:
    case ConstRawPtr:
        return;
    case RefPtr:
    case ConstRefPtr:
        (*(ref<const minitl::refcountable>*)ptr).~ref();
        return;
    case WeakPtr:
    case ConstWeakPtr:
        (*(weak<const minitl::refcountable>*)ptr).~weak();
        return;
    default:
        be_notreached();
        return;
    }
}

const RTTI::PropertyInfo* TypeInfo::property(const char *name) const
{
    for(const RTTI::ClassInfo* klass = metaclass.m_ptr; klass != 0; klass = klass->parent.m_ptr)
    {
        for(size_t i = 0; i < klass->propertyCount; ++i)
        {
            if(strcmp(name, klass->properties.m_ptr[i].name.m_ptr) == 0)
                return &klass->properties.m_ptr[i];
        }
    }
    return 0;
}

}

#include <rtti/value.hh>

static struct A
{
    A()
    {
        using namespace BugEngine;
        // local copy
        RTTI::ClassInfo ci = *be_typeid<RTTI::ClassInfo>().klass;
        Value v(Value::ByRef(ci));
        Value v2(Value::ByRef(*be_typeid<RTTI::ClassInfo>().klass));
        OutputDebugString((::minitl::format<>("type: %s\n") | v.type().name()).c_str());
        Value name = v("name");
        OutputDebugString((::minitl::format<>("name: %s\n") | name.as< const raw<const char> >().m_ptr).c_str());
        OutputDebugString((::minitl::format<>("name: %s\n") | ci.name.m_ptr).c_str());
        raw<const char> c = {"blabla"};
        name = c;
        OutputDebugString((::minitl::format<>("name: %s\n") | name.as< raw<const char> >().m_ptr).c_str());
        OutputDebugString((::minitl::format<>("name: %s\n") | ci.name.m_ptr).c_str());
        v("name") = c;
        OutputDebugString((::minitl::format<>("name: %s\n") | name.as< raw<const char> >().m_ptr).c_str());
        OutputDebugString((::minitl::format<>("name: %s\n") | ci.name.m_ptr).c_str());

        RTTI::ClassInfo root = *be_typeid<void>().klass;
    }
    ~A()
    {
    }
} s_a;
