/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_DOCUMENTATION_BRIEF_HH_
#define BE_RTTI_DOCUMENTATION_BRIEF_HH_
/*****************************************************************************/

namespace BugEngine { namespace Documentation
{

struct Brief
{
    message doc;
    Brief(const char *brief);
    ~Brief();
};

}}

/*****************************************************************************/
#endif
