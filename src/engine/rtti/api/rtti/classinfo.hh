/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>
#include   <rtti/value.hh>

namespace BugEngine
{

class PropertyInfo;

class ClassInfo : public TypeInfo
{
public:
    ClassInfo(const inamespace& name);
    virtual ~ClassInfo();
};

}

/*****************************************************************************/
#endif
