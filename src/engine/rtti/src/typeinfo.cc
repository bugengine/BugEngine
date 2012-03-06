/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine
{

u32 Type::size() const
{
    switch(indirection)
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

minitl::format<> Type::name() const
{
    minitl::format<> n("");
    if (indirection && (indirection & MutableBit))
        n = minitl::format<>("%s") | metaclass->name;
    else
        n = minitl::format<>("const %s") | metaclass->name;

    switch(indirection & IndirectionMask)
    {
    case Class:
        break;
    case ConstRawPtr:
        n = minitl::format<>("raw<%s>") | n;
        break;
    case ConstWeakPtr:
        n = minitl::format<>("weak<%s>") | n;
        break;
    case ConstRefPtr:
        n = minitl::format<>("ref<%s>") | n;
        break;
    default:
        be_notreached();
        break;
    }

    switch(constness)
    {
    case Const:
        return minitl::format<>("const %s") | n;
    case Mutable:
        return n;
    default:
        be_notreached();
        return n;
    }
}

void* Type::rawget(const void* data) const
{
    switch(indirection)
    {
    case Class:
        return (void*)data;
    case RawPtr:
    case ConstRawPtr:
        return *(void**)data;
    case RefPtr:
    case ConstRefPtr:
        return ((ref<minitl::refcountable>*)data)->operator->();
    case WeakPtr:
    case ConstWeakPtr:
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

void Type::destroy(void* ptr) const
{
    switch(indirection)
    {
    case Class:
        return metaclass->destroy(ptr);
    case RawPtr:
    case ConstRawPtr:
        return;
    case RefPtr:
    case ConstRefPtr:
        ((ref<const minitl::refcountable>*)ptr)->~ref();
        return;
    case WeakPtr:
    case ConstWeakPtr:
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
    if (constness < other.constness)
        return 1000000;
    else
        result += constness - other.constness;
    if ((indirection & IndirectionMask) < (other.indirection & IndirectionMask))
        return 1000000;
    else
        result += (indirection & IndirectionMask) - (other.indirection & IndirectionMask);
    if ((indirection & MutableBit) < (other.indirection & MutableBit))
        return 1000000;
    else
        result += (indirection & MutableBit) - (other.indirection & MutableBit);
    return result + metaclass->distance(other.metaclass);
}

}
