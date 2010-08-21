/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <devices/keyboard.hh>
#include    <devicecontrol.hh>
#include    <groupcontrol.hh>

namespace BugEngine { namespace Input
{

size_t Keyboard::s_keyboardCount = 0;

/*static Device& getKeyboard()
{
    static Device s_keyboard("Keyboard", 256);
    return s_keyboard;
}*/

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
            ref<Control> ctrl = ref<DeviceControl>::create<Arena::General>(mappedName, this, i);
            addControl(ctrl);
            addControlAlias("US", keyName, ctrl);
            /* TODO Value v = getKeyboard().getNamespace()->get(istring(mappedName));
            if(v)
            {
                v.as< ref<GroupControl> >()->add(ctrl);
            }
            else
            {
                ref<GroupControl> g = ref<GroupControl>::create<Arena::General>(mappedName);
                getKeyboard().addControl(g);
                getKeyboard().addControlAlias("US", keyName, g);
                g->add(ctrl);
            }*/
        }
    }
}

Keyboard::~Keyboard()
{
}

}}
