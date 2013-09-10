/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <sys/types.h>
#include    <sys/sysctl.h>


namespace BugEngine
{

size_t Environment::getProcessorCount() const
{
    int cpuCount = 0;
    size_t len = sizeof(cpuCount);

    int mib[4];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    if (sysctl(mib, 2, &cpuCount, &len, NULL, 0) == -1)
    {
        be_error("Could not retrieve number of processors: %s" | sys_errlist[errno]);
        cpuCount = 1;
    }
    be_assert_recover(cpuCount >= 1, "Invalid number of CPUs returned by sysctl: %d" | cpuCount, cpuCount=1);
    be_info("found %d CPU"|cpuCount);
    return cpuCount;
}

}