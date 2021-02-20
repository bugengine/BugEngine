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

private:
    static minitl::vector< raw< const RTTI::Class > >& parameterClasses();

protected:
    IParameter();
    ~IParameter();

public:
    weak< const IMemoryBuffer > getCurrentBank() const;
    weak< const IMemoryBuffer > getBank(weak< const IMemoryHost > host) const;

    virtual ref< IProduct > makeProduct(ref< IParameter > parameter, weak< Task::ITask > task) = 0;

    static raw< const RTTI::Class > getParameterClass(const istring parameterTypeName);
    static const istring            getProductTypePropertyName();

    struct be_api(SCHEDULER) ParameterRegistration
    {
    private:
        raw< const RTTI::Class > m_class;

    public:
        ParameterRegistration(raw< const RTTI::Class > klass);
        ~ParameterRegistration();
    };
    friend struct ParameterRegistration;
};

}  // namespace KernelScheduler
}  // namespace BugEngine

/**************************************************************************************************/
#endif
