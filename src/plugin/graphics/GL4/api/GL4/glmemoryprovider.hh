/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_GLMEMORYPROVIDER_HH_
#define BE_GL4_GLMEMORYPROVIDER_HH_
/*****************************************************************************/
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine { namespace OpenGL
{

class be_api(GL4) GLMemoryProvider : public Kernel::IMemoryProvider
{
public:
    GLMemoryProvider();
    ~GLMemoryProvider();
protected:
    virtual Kernel::KernelParameter getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const override;
};

}}

/*****************************************************************************/
#endif
