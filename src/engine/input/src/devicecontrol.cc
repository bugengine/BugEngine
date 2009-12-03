/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <devicecontrol.hh>
#include    <input/device.hh>


namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",DeviceControl);

DeviceControl::DeviceControl(const istring& name, weak<const Device> device, size_t index)
:   Control(name)
,   m_device(device)
,   m_index(index)
{
}

DeviceControl::~DeviceControl()
{
}

float DeviceControl::value() const
{
    return m_device->getActiveBuffer()[m_index];
}

bool DeviceControl::isDown() const
{
    return m_device->getActiveBuffer()[m_index] > 0.0f;
}

bool DeviceControl::wasDown() const
{
    return m_device->getLastBuffer()[m_index] > 0.0f;
}

bool DeviceControl::wentDown() const
{
    return isDown() && !wasDown();
}

bool DeviceControl::wentUp() const
{
    return wasDown() && !isDown();
}

}}
