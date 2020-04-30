/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.GL4/glmemoryhost.hh>

namespace BugEngine { namespace OpenGL {

GLMemoryHost::GLMemoryHost() : IMemoryHost("OpenGL")
{
}

GLMemoryHost::~GLMemoryHost()
{
}

void GLMemoryHost::release(weak< KernelScheduler::IMemoryBuffer > buffer)
{
    be_forceuse(buffer);
}

}}  // namespace BugEngine::OpenGL
