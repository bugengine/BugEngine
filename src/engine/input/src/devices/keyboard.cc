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
#include    <devices/keyboard.hh>
#include    <devicecontrol.hh>
#include    <groupcontrol.hh>

namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",Keyboard);

size_t Keyboard::s_keyboardCount = 0;

static Device& getKeyboard()
{
    static Device s_keyboard("Keyboard", 256);
    return s_keyboard;
}

Keyboard::Keyboard(size_t /*numControls*/)
:   Device((minitl::format<>("Keyboard%u") | s_keyboardCount++).c_str(), 256)
{
    HKL oldLayout = GetKeyboardLayout(GetCurrentThreadId());
    static HKL usLayout = LoadKeyboardLayoutA("00000409", 0);

    char keyName[256];
    char mappedName[256];
    for(LONG i = 0; i < 256; ++i)
    {
        if(GetKeyNameTextA(i<<16, mappedName, 256) != 0)
        {
            ActivateKeyboardLayout(usLayout, 0);
            GetKeyNameTextA(i<<16, keyName, 256);
            ActivateKeyboardLayout(oldLayout, 0);
            Control* ctrl = new DeviceControl(mappedName, this, i);
            addControl(ctrl);
            addControlAlias("US", keyName, ctrl);
            Value v = getKeyboard().getNamespace()->get(istring(mappedName));
            if(v)
            {
                v.as< refptr<GroupControl> >()->add(ctrl);
            }
            else
            {
                GroupControl* g = new GroupControl(mappedName);
                getKeyboard().addControl(g);
                getKeyboard().addControlAlias("US", keyName, g);
                g->add(ctrl);
            }
        }
    }
}

Keyboard::~Keyboard()
{
}

}}
