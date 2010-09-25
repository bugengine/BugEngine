/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_INL_
#define BE_RTTI_TYPEINFO_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

TypeInfo::TypeInfo(const TypeInfo& proxy, Type type, size_t count)
:   m_class(proxy.m_class)
{
    memcpy(m_attributes, proxy.m_attributes, sizeof(m_attributes));
    for(size_t i = 0; i < s_maxAttributeCount; ++i)
    {
        if(m_attributes[i].type == Class)
        {
            m_attributes[i].type = type;
            m_attributes[i].count = count;
            return;
        }
    }
    be_notreached();
}

TypeInfo::TypeInfo(const ClassInfo& klass)
:   m_class(klass)
{
    memset(m_attributes, 0, sizeof(m_attributes));
}

TypeInfo::~TypeInfo()
{
}

}}


/*****************************************************************************/
#endif
