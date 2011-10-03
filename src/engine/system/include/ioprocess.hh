/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_IOPROCESS_HH_
#define BE_SYSTEM_IOPROCESS_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>
#include    <system/file/file.script.hh>

namespace BugEngine
{

struct IOContext
{
    IOContext();
    ~IOContext();

    static intptr_t ioProcess(intptr_t p1, intptr_t p2);
    static void     pushTicket(ref<const File::Ticket> ticket);
};

}

/*****************************************************************************/
#endif
