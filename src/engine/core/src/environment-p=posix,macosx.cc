/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <unistd.h>
#if !defined(BE_PLATFORM_LINUX) && !defined(BE_PLATFORM_SUN)
# include   <sys/types.h>
# include   <sys/sysctl.h>
#endif
#include    <cerrno>
#include    <cstdio>


namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory("share/bugengine")
,   m_game("")
,   m_user(getenv("USER"))
{
    m_homeDirectory.push_back(".bugengine");
}

void Environment::init(int argc, const char *argv[])
{
    m_game = istring("sample.kernel");
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
    while (*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    ipath rootdir = ipath(argv[0], filename);
    for (u32 i = 0; i < rootdir.size(); ++i)
    {
        chdir(rootdir[i].c_str());
    }
    for (int arg = 1; arg < argc; arg++)
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }
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
#if defined(BE_PLATFORM_LINUX) || defined(BE_PLATFORM_SUN)
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    int cpuCount = 0;
    size_t len = sizeof(cpuCount);

    int mib[4];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
#if defined(BE_PLATFORM_MACOS)
    if (sysctlbyname("hw.physicalcpu", &cpuCount, &len, NULL, 0) == -1)
#endif
    if (sysctl(mib, 2, &cpuCount, &len, NULL, 0) == -1)
    {
        be_error("Could not retrieve number of processors: %s" | sys_errlist[errno]);
        cpuCount = 1;
    }
    be_assert_recover(cpuCount >= 1, "Invalid number of CPUs returned by sysctl: %d" | cpuCount, cpuCount=1);
    be_info("found %d CPU"|cpuCount);
    return cpuCount;
#endif
}

}
