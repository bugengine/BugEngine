/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_WAITABLE_HH_
#define BE_CORE_THREADS_WAITABLE_HH_
/*****************************************************************************/

namespace BugEngine { namespace Threads
{

class COREEXPORT Waitable
{
public:
    Waitable()          {}
    virtual ~Waitable() {}


    enum WaitResult
    {
        Finished,
        TimeOut,
        Abandoned
    };
    enum
    {
        Forever = 0xffffffff
    };
    virtual WaitResult wait(unsigned int waitTime = Forever) = 0;
};

}}

/*****************************************************************************/
#endif
