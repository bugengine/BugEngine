/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_STRUCT_INL_
#define BE_RTTI_ENGINE_STRUCT_INL_
/*****************************************************************************/
#include <rtti/engine/property.hh>

namespace BugEngine { namespace RTTI
{

template< typename Class, typename Parent >
StructInfo<Class, Parent>::StructInfo(const inamespace& name)
:   BugEngine::ClassInfo(name)
{
}

template< typename Class, typename Parent >
StructInfo<Class, Parent>::~StructInfo()
{
}

template< typename Class, typename Parent >
template< typename T >
PropertyInfo& StructInfo<Class, Parent>::property(const istring& name, T(Class::*member))
{
    ref< FieldPropertyInfo<Class, T> > prop = ref< FieldPropertyInfo<Class, T> >::create(member);
    m_properties.push_back(std::make_pair(name, prop));
    return *prop;
}

}}


/*****************************************************************************/
#endif
