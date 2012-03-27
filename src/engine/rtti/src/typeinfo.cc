/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI
{

u32 Type::size() const
{
    switch(indirection)
    {
    case Value:
        return metaclass->size;
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

minitl::format<> Type::name() const
{
    minitl::format<> n(metaclass->name.str());
    switch(access)
    {
    case Const:
        n.append(" const");
        break;
    case Mutable:
        break;
    default:
        be_notreached();
    }

    switch(indirection)
    {
    case Value:
        break;
    case RawPtr:
        n = minitl::format<>("raw<%s>") | n;
        break;
    case WeakPtr:
        n = minitl::format<>("weak<%s>") | n;
        break;
    case RefPtr:
        n = minitl::format<>("ref<%s>") | n;
        break;
    default:
        be_notreached();
        break;
    }

    switch(constness)
    {
    case Const:
        n.append(" const");
        break;
    case Mutable:
        break;
    default:
        be_notreached();
    }
    return n;
}

void* Type::rawget(const void* data) const
{
    switch(indirection)
    {
    case Value:
        return (void*)data;
    case RawPtr:
        return *(void**)data;
    case RefPtr:
        return ((ref<minitl::refcountable>*)data)->operator->();
    case WeakPtr:
        return ((weak<minitl::refcountable>*)data)->operator->();
    default:
        be_notreached();
        return 0;
    }
}

void Type::copy(const void* source, void* dest) const
{
    switch(indirection)
    {
    case Value:
        return metaclass->copy(source, dest);
    case RawPtr:
        memcpy(dest, source, sizeof(void*));
        return;
    case RefPtr:
        new (dest) const ref<const minitl::refcountable>(*(const ref<const minitl::refcountable>*)source);
        return;
    case WeakPtr:
        new (dest) const weak<const minitl::refcountable>(*(const weak<const minitl::refcountable>*)source);
        return;
    default:
        be_notreached();
        return;
    }
}

void Type::destroy(void* ptr) const
{
    switch(indirection)
    {
    case Value:
        return metaclass->destroy(ptr);
    case RawPtr:
        return;
    case RefPtr:
        ((ref<const minitl::refcountable>*)ptr)->~ref();
        return;
    case WeakPtr:
        ((weak<const minitl::refcountable>*)ptr)->~weak();
        return;
    default:
        be_notreached();
        return;
    }
}

u32 Type::distance(const Type& other) const
{
    u32 result = 0;
    if (access < other.access)
        return 1000000;
    else
        result += access - other.access;
    if (indirection < other.indirection)
        return 1000000;
    else
        result += indirection - other.indirection;
    return result + metaclass->distance(other.metaclass);
}

}}
