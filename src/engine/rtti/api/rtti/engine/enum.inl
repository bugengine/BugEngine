/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ENUM_INL_
#define BE_RTTI_ENGINE_ENUM_INL_
/*****************************************************************************/
#include <rtti/engine/property.hh>

namespace BugEngine { namespace RTTI
{

template< typename Enum >
EnumInfo<Enum>::EnumInfo(const inamespace& name)
:   BugEngine::EnumInfo(name)
{
}

template< typename Enum >
EnumInfo<Enum>::~EnumInfo()
{
}

template< typename Enum >
PropertyInfo& EnumInfo<Enum>::value(istring name, Enum value)
{
    ref< ConstPropertyInfo<Enum> > prop = ref< ConstPropertyInfo<Enum> >::create(value);
    m_properties.push_back(std::make_pair(name, prop));
    return *prop;
}


}}


/*****************************************************************************/
#endif
