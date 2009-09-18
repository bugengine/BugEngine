/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_POP_HH_
#define BE_SYSTEM_POP_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class SYSTEMEXPORT Pop : public Action
{
public:
    Pop();
    ~Pop();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEMEXPORT,Pop,Action)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
