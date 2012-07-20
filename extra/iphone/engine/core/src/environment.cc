/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <core/debug/logger.hh>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/sysctl.h>
#include    <errno.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory("share/bugengine")
,   m_game("bugeditor")
,   m_user(getenv("USER"))
{
    m_homeDirectory.push_back(BugEngine::Debug::Format<>(".bugengine"));
}

void Environment::init(int argc, const char *argv[])
{
    const char* filename = argv[0];
    while (*filename != 0)
    {
        filename++;
    }
    while (*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    filename--;
    for( int arg = 1; arg < argc; arg++ )
    {
        printf("%s\n", argv[arg]);
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
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

size_t Environment::getProcessorCount() const
{
#if defined(BE_PLATFORM_LINUX) || defined(BE_PLATFORM_SUN) || defined(BE_PLATFORM_MACOS)
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    int numCPU = 0;
    int mib[4];
    size_t len = sizeof(numCPU);

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    if (sysctl(mib, 2, &numCPU, &len, NULL, 0) == -1)
    {
        be_error("Could not retrieve number of processors: %s" | sys_errlist[errno]);
        numCPU = 1;
    }
    be_assert_recover(numCPU >= 1, "Weird number of CPUs returned by sysctl: %d" | numCPU, numCPU=1);
    return numCPU;
#endif
}

}
