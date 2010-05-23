/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENUMINFO_HH_
#define BE_RTTI_ENUMINFO_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

class EnumInfo : public TypeInfo
{
protected:
    minitl::vector< std::pair< istring, int > >   m_enum;
public:
    EnumInfo(const inamespace& name);
    virtual ~EnumInfo();
};

}

/*****************************************************************************/
#endif
