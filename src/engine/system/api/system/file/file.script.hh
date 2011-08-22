/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_FILE_SCRIPT_HH_
#define BE_SYSTEM_FILE_FILE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine
{

class be_api(SYSTEM) File : public minitl::refcountable
{
published:
    File();
    ~File();
};

}

/*****************************************************************************/
#endif
