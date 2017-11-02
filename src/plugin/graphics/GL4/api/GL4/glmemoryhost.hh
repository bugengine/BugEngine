/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_GLMEMORYHOST_HH_
#define BE_GL4_GLMEMORYHOST_HH_
/**************************************************************************************************/
#include    <GL4/stdafx.h>
#include    <scheduler/kernel/imemoryhost.hh>

namespace BugEngine { namespace OpenGL
{

class be_api(GL4) GLMemoryHost : public Kernel::IMemoryHost
{
public:
    GLMemoryHost();
    ~GLMemoryHost();
};

}}

/**************************************************************************************************/
#endif
