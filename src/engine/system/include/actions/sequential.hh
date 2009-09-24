/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SEQUENTIAL_HH_
#define BE_SYSTEM_SEQUENTIAL_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class be_api(SYSTEM) Sequential : public Action
{
public:
    Sequential();
    ~Sequential();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEM,Sequential,Action)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
