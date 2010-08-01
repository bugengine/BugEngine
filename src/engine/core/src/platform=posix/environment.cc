/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <core/debug/logger.hh>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/sysctl.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory("")
,   m_game("")
,   m_user(getenv("USER"))
{
    FILE* cmdline = fopen("/proc/self/cmdline", "r");
    if(!cmdline)
        return;
    char exe[4096];
    fread(exe, 1, 4096, cmdline);
    char* filename = exe;
    while(*filename != 0)
    {
        filename++;
    }
    while(*filename != '/' && filename != exe)
    {
        filename--;
    }
    *filename = 0;
    m_game = filename+1;
    m_dataDirectory = (minitl::format<4096>("%s/../share/bugengine") | exe).c_str();
}

Environment::~Environment()
{
}

const Environment& Environment::getEnvironment()
{
    static Environment s_environment;
    return s_environment;
}

const ipath& Environment::getDataDirectory() const
{
    return m_dataDirectory;
}

const ipath& Environment::getHomeDirectory() const
{
    return m_homeDirectory;
}

const istring& Environment::getGame() const
{
    return m_game;
}

const istring& Environment::getUser() const
{
    return m_user;
}
size_t Environment::getProcessorCount() const
{
#if defined(BE_PLATFORM_LINUX) || defined(BE_PLATFORM_SUNOS)
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    int numCPU;
    int mib[4];
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if( numCPU < 1 )
    {
         mib[1] = HW_AVAILCPU;
         sysctl( mib, 2, &numCPU, &len, NULL, 0 );

         if( numCPU < 1 )
         {
              numCPU = 1;
         }
    }
    return numCPU;
#endif
}

}
