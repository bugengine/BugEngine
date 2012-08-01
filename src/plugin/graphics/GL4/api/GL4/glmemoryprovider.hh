/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_GLMEMORYPROVIDER_HH_
#define BE_GL4_GLMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <core/memory/kernel/imemoryprovider.hh>

namespace BugEngine { namespace OpenGL
{

class GLMemoryProvider : public Kernel::IMemoryProvider
{
public:
    GLMemoryProvider();
    ~GLMemoryProvider();
};

}}

/*****************************************************************************/
#endif
