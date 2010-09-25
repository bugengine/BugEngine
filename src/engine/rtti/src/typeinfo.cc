/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>
#include   <rtti/classinfo.hh>

namespace BugEngine { namespace RTTI
{

size_t TypeInfo::size() const
{
    size_t size = m_class.size;
    for(size_t i = 0; i < s_maxAttributeCount; ++i)
    {
        switch(m_attributes[i].type)
        {
        case Class:
            return size;
        case Array:
            size = size * m_attributes[i].count;
            break;
        case Ref:
        case Ptr:
            size = sizeof(void*);
            break;
        case RefPtr:
            size = sizeof(ref<minitl::refcountable>);
            break;
        case WeakPtr:
            size = sizeof(weak<minitl::refcountable>);
            break;
        case Const:
            break;
        default:
            be_notreached();
        }
    }
    return size;
}

void TypeInfo::copy(const void* /*src*/, void* /*dst*/) const
{
}

void TypeInfo::destroy(void* /*src*/) const
{
}

}}
