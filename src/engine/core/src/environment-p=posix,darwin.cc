/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <core/debug/logger.hh>
#include    <unistd.h>
#if !defined(BE_PLATFORM_LINUX) && !defined(BE_PLATFORM_SUN)
# include    <sys/types.h>
# include    <sys/sysctl.h>
#endif

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory("../share/bugengine")
,   m_game("")
,   m_user(getenv("USER"))
{
}

void Environment::init(int argc, const char *argv[])
{
    const char* filename = argv[0];
    while(*filename != 0)
    {
        filename++;
    }
    while(*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    m_game = filename+1;
    filename--;
    while(*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    m_dataDirectory = ipath(argv[0], filename);
    m_dataDirectory += "share";
    m_dataDirectory += "bugengine";
}

Environment::~Environment()
{
}

Environment& Environment::getEnvironment()
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
#if defined(BE_PLATFORM_LINUX) || defined(BE_PLATFORM_SUN) || defined(BE_PLATFORM_MACOS)
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    int numCPU = 0;
    int mib[4];
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    return numCPU;
#endif
}

}
