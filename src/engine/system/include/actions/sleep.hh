/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SLEEP_HH_
#define BE_SYSTEM_SLEEP_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class be_api(SYSTEM) Sleep : public Action
{
public:
    Sleep();
    ~Sleep();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEM,Sleep,Action)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
