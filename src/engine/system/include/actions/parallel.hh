/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_PARALLEL_HH_
#define BE_SYSTEM_PARALLEL_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class be_api(SYSTEM) Parallel : public Action
{
public:
    Parallel();
    ~Parallel();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEM,Parallel,Action)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
