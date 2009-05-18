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

#ifndef BE_INPUT_DEVICECONTROL_HH_
#define BE_INPUT_DEVICECONTROL_HH_
/*****************************************************************************/
#include    <input/control.hh>

namespace BugEngine { namespace Input
{

class Device;

class DeviceControl : public Control
{
private:
    const Device*   m_device;
    size_t          m_index;
public:
    DeviceControl(const istring& name,const  Device* device, size_t index);
    ~DeviceControl();

    float           value() const override;
    bool            isDown() const override;
    bool            wasDown() const override;
    bool            wentDown() const override;
    bool            wentUp() const override;
    size_t          index() const { return m_index; }

    METACLASS(INPUTEXPORT,DeviceControl,Control)
    PROPERTIES
    END
};

}}

/*****************************************************************************/
#endif
