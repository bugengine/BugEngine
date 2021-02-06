/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_GLMEMORYHOST_HH_
#define BE_GL4_GLMEMORYHOST_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace OpenGL {

class be_api(GL4) GLMemoryHost : public KernelScheduler::IMemoryHost
{
public:
    GLMemoryHost();
    ~GLMemoryHost();

    void release(weak< KernelScheduler::IMemoryBuffer > buffer);
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
