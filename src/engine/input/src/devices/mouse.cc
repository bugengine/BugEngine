/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <input/stdafx.h>
#include    <devices/mouse.hh>
#include    <devicecontrol.hh>
#include    <groupcontrol.hh>

namespace BugEngine { namespace Input
{

ABSTRACTMETACLASS_IMPL("Input",Mouse);

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
