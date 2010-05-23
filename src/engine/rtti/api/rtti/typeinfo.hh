/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_HH_
#define BE_RTTI_TYPEINFO_HH_
/*****************************************************************************/
#include   <rtti/value.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine
{

struct TypeInfo
{
public:
    TypeInfo(const TypeInfo* );
};

template<typename T>
TypeInfo be_typeid();

}

#include   <rtti/value.inl>

/*****************************************************************************/
#endif
