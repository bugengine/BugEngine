/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

TypeInfo::TypeInfo(const inamespace &name)
    :   name(name)
{
}

TypeInfo::~TypeInfo()
{
}

ref<const PropertyInfo> TypeInfo::getProperty(istring prop) const
{
    for(minitl::vector< std::pair< istring, ref<const PropertyInfo> > >::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        if(it->first == prop)
            return it->second;
    }
    return ref<const PropertyInfo>();
}

}
