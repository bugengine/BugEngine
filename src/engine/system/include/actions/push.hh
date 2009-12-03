/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PUSH_HH_
#define BE_SYSTEM_PUSH_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class be_api(SYSTEM) Push : public Action
{
private:
    Value  m_value;
public:
    Push();
    Push(const Value& value);
    ~Push();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEM,Push,Action)
        ref<Push> createWithValue(const Value& value);
    be_properties
        be_classmethod(createWithValue);

        be_property(value)
            [be_read(m_value)]
            [be_write(m_value)];
    be_end
};

}}

/*****************************************************************************/
#endif
