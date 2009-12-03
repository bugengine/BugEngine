/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_GROUPCONTROL_HH_
#define BE_INPUT_GROUPCONTROL_HH_
/*****************************************************************************/
#include    <input/control.hh>

namespace BugEngine { namespace Input
{

class GroupControl : public Control
{
private:
    std::vector< ref<Control> >  m_controls;
public:
    GroupControl(const istring& name);
    ~GroupControl();

    float           value() const override;
    bool            isDown() const override;
    bool            wasDown() const override;
    bool            wentDown() const override;
    bool            wentUp() const override;

    void add(ref<Control> ctrl);

    be_metaclass(NONE,GroupControl,Control)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
