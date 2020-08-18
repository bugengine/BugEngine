/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/environment.hh>

#include <unistd.h>

namespace BugEngine {

size_t Environment::getProcessorCount() const
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

}  // namespace BugEngine
