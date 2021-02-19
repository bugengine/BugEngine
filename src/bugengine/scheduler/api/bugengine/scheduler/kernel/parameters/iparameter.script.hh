/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IPARAMETER_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IPARAMETER_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>

namespace BugEngine {

namespace Task {

class ITask;

}

namespace KernelScheduler {

class IMemoryBuffer;
class IMemoryHost;
class IProduct;

class be_api(SCHEDULER) IParameter : public minitl::refcountable
{
private:
    enum
    {
        BufferCount = 2
    };
    ref< const IMemoryBuffer > m_buffers[BufferCount];

protected:
    IParameter();
    ~IParameter();

public:
    weak< const IMemoryBuffer > getCurrentBank() const;
    weak< const IMemoryBuffer > getBank(weak< const IMemoryHost > host) const;

    static raw< RTTI::Class > getNamespace();
    static const istring      getProductTypePropertyName();

    virtual ref< IProduct > makeProduct(ref< IParameter > parameter, weak< Task::ITask > task) = 0;
};

}  // namespace KernelScheduler
}  // namespace BugEngine

/**************************************************************************************************/
#endif
