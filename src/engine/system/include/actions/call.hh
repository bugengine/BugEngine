/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_CALL_HH_
#define BE_SYSTEM_CALL_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class be_api(SYSTEM) Call : public Action
{
private:
    weak<RTTI::Method>   m_method;
public:
    Call();
    Call(weak<RTTI::Method> method);
    ~Call();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEM,Call,Action)
        ref<Call> createWithMethod(weak<RTTI::Method> method);
    be_properties
        be_classmethod(createWithMethod);

        be_property(method)
            [be_read(m_method)]
            [be_write(m_method)];
    be_end
};

}}

/*****************************************************************************/
#endif
