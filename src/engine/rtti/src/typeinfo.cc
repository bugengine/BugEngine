/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

size_t TypeInfo::size() const
{
    size_t size = m_class->size();
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

std::string TypeInfo::name() const
{
    std::string name = m_class->name();
    for(size_t i = 0; i < s_maxAttributeCount; ++i)
    {
        switch(m_attributes[i].type)
        {
        case Class:
            return name;
        case Array:
            name = name+(minitl::format<>("[%u]")|m_attributes[i].count).c_str();
            break;
        case Ref:
            name = name+"&";
            break;
        case Ptr:
            name = name+"*";
            break;
        case RefPtr:
            name = "ref<"+name+">";
            break;
        case WeakPtr:
            name = "weak<"+name+">";
            break;
        case Const:
            name = name+" const";
            break;
        default:
            be_notreached();
        }
    }
    return name;
}

}
