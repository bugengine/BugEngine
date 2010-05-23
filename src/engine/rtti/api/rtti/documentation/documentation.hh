/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_DOCUMENTATION_DOCUMENTATION_HH_
#define BE_RTTI_DOCUMENTATION_DOCUMENTATION_HH_
/*****************************************************************************/

namespace BugEngine { namespace Documentation
{

class Doc
{
public:
    message doc;
    Doc(const char *doc);
    ~Doc();
};

}}

/*****************************************************************************/
#endif
