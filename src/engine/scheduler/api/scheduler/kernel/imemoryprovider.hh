/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Kernel
{

class IMemoryBank;
struct KernelParameter
{
    void*   p1;
    void*   p2;
};

class be_api(SCHEDULER) IMemoryProvider : public minitl::pointer
{
private:
    istring const   m_name;
protected:
    IMemoryProvider(const istring& name);
public:
    istring name() const { return m_name; }

    virtual Kernel::KernelParameter getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const = 0;
};

}}

/*****************************************************************************/
#endif
