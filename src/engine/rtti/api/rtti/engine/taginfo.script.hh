/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TAGINFO_HH_
#define BE_RTTI_TAGINFO_HH_
/*****************************************************************************/
#include    <rtti/value.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

struct be_api(RTTI) Tag
{
    raw<Tag> const next;
    Value tag;
};

}}

/*****************************************************************************/
#endif

