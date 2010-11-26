/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef _CORE_DEBUG_EXCEPTION_H_
#define _CORE_DEBUG_EXCEPTION_H_
/*****************************************************************************/

namespace BugEngine
{

class be_api(CORE) Exception
{
public:
    const minitl::format<> what;
    Exception(const minitl::format<> msg)
        :   what(msg)
    {
    }
};

}

/*****************************************************************************/
#endif
