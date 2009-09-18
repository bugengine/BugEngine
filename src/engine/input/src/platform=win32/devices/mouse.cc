/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <devices/mouse.hh>
#include    <devicecontrol.hh>
#include    <groupcontrol.hh>

namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",Mouse);

size_t Mouse::s_miceCount = 0;

static Device& getMouse()
{
    static Device s_mouse("Mouse", 256);
    return s_mouse;
}

Mouse::Mouse(size_t numButtons)
:   Device((minitl::format<>("Mouse%u") | s_miceCount++).c_str(), numButtons+2)
{
    for(size_t i = 0; i < numButtons; ++i)
    {
        minitl::format<> f = minitl::format<>("Button %u") | i;
        Control* ctrl = new DeviceControl(f, this, i);
        addControl(ctrl);
        Value v = getMouse().getNamespace()->get(f);
        if(v)
        {
            v.as< refptr<GroupControl> >()->add(ctrl);
        }
        else
        {
            GroupControl* g = new GroupControl(f);
            getMouse().addControl(g);
            g->add(ctrl);
        }
    }
}

Mouse::~Mouse()
{
}

}}
