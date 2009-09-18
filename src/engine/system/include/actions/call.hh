/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_CALL_HH_
#define BE_SYSTEM_CALL_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class SYSTEMEXPORT Call : public Action
{
private:
    RTTI::Method*   m_method;
public:
    Call();
    Call(RTTI::Method* method);
    ~Call();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEMEXPORT,Call,Action)
        refptr<Call> createWithMethod(RTTI::Method* method);
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
