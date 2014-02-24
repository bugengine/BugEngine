/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
#define BE_SCHEDULER_KERNEL_IMEMORYPROVIDER_HH_
/**************************************************************************************************/

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
    friend class IMemoryBank;
private:
    istring const   m_name;
protected:
    IMemoryProvider(const istring& name);
    virtual Kernel::KernelParameter getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const = 0;
public:
    istring name() const { return m_name; }

};

}}

/**************************************************************************************************/
#endif
