/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>
#include   <rtti/engine/propertyinfo.script.hh>

namespace BugEngine
{

size_t TypeInfo::size() const
{
    switch(type)
    {
    case Class:
        return metaclass->size;
    case Reference:
        return sizeof(void*);
    case RawPtr:
        return sizeof(raw<char>);
    case RefPtr:
        return sizeof(ref<minitl::refcountable>);
    case WeakPtr:
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
    case Reference:
        return minitl::format<>("%s&") | n;
    case RawPtr:
        return minitl::format<>("raw<%s>") | n;
    case RefPtr:
        return minitl::format<>("ref<%s>") | n;
    case WeakPtr:
        return minitl::format<>("weak<%s>") | n;
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
    case Reference:
    case RawPtr:
        memcpy(dest, source, sizeof(void*));
        return;
    case RefPtr:
        new (dest) ref<minitl::refcountable>(*(ref<minitl::refcountable>*)source);
        return;
    case WeakPtr:
        new (dest) weak<minitl::refcountable>(*(weak<minitl::refcountable>*)source);
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
    case Reference:
    case RawPtr:
        return;
    case RefPtr:
        (*(ref<minitl::refcountable>*)ptr).~ref();
        return;
    case WeakPtr:
        (*(weak<minitl::refcountable>*)ptr).~weak();
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
        ::BugEngine::Value v (*::BugEngine::be_typeid<BugEngine::RTTI::ClassInfo>().klass);
        OutputDebugString((::minitl::format<>("type: %s\n") | v.type().name()).c_str());
        ::BugEngine::Value name = v("name");
        OutputDebugString((::minitl::format<>("name: %s\n") | name.as< raw<const char> >().m_ptr).c_str());
    }
    ~A()
    {
    }
} s_a;
