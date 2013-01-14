/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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

minitl::format<1024u> Type::name() const
{
    static const char *constnessPrefix[] = {
        "const ",
        ""
    };
    static const char* indirectionPrefix[] = {
        "",
        "raw<",
        "weak<",
        "ref<"
    };
    static const char* indirectionSuffix[] = {
        "",
        ">",
        ">",
        ">"
    };
    return minitl::format<1024u>("%s%s%s%s")
            | indirectionPrefix[indirection]
            | constnessPrefix[constness]
            | metaclass->name
            | indirectionSuffix[indirection];
}

bool Type::isA(const Type& other) const
{
    return     other.indirection <= indirection
            && indirection*other.access <= indirection*access
            && metaclass->isA(other.metaclass);
}

bool operator==(Type t1, Type t2)
{
    return t1.metaclass == t2.metaclass && t1.indirection == t2.indirection && t1.access == t2.access && t1.constness == t2.constness;
}

bool operator<=(Type t1, Type t2)
{
    return     (t1.indirection <= t2.indirection)
            && t1.access <= t2.access
            && t2.metaclass->isA(t1.metaclass);
}

}}
