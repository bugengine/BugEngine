/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/
#include   <rtti/classinfo.hh>

namespace BugEngine
{

TypeInfo::TypeInfo(const TypeInfo& proxy, Type type, size_t count)
:   m_type(type)
,   m_count(count)
,   m_class(proxy.m_class)
{
}

TypeInfo::TypeInfo(weak<const ClassInfo> klass)
:   m_type(Class)
,   m_class(klass)
{
}

TypeInfo::~TypeInfo()
{
}

void TypeInfo::copy(const void* /*src*/, void* /*dst*/) const
{
}

void TypeInfo::destroy(void* /*src*/) const
{
}

size_t TypeInfo::size() const
{
    return m_class->size() * m_count;
}

}


/*****************************************************************************/
#endif
