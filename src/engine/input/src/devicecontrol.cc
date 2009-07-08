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
#include    <devicecontrol.hh>
#include    <input/device.hh>


namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",DeviceControl);

DeviceControl::DeviceControl(const istring& name, const Device* device, size_t index)
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
