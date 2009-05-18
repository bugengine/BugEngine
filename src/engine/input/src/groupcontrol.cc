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
#include    <groupcontrol.hh>
#include    <input/device.hh>


namespace BugEngine { namespace Input
{

ABSTRACTMETACLASS_IMPL("Input",GroupControl);

GroupControl::GroupControl(const istring& name)
:   Control(name)
{
}

GroupControl::~GroupControl()
{
}

float GroupControl::value() const
{
    float value = 0.0f;
    for(size_t i = 0; i < m_controls.size(); ++i)
        value += m_controls[i]->value();
    return clamp(value, -1.0f, 1.0f);
}

bool GroupControl::isDown() const
{
    for(size_t i = 0; i < m_controls.size(); ++i)
        if(m_controls[i]->isDown())
            return true;
    return false;
}

bool GroupControl::wasDown() const
{
    for(size_t i = 0; i < m_controls.size(); ++i)
        if(m_controls[i]->wasDown())
            return true;
    return false;
}

bool GroupControl::wentDown() const
{
    for(size_t i = 0; i < m_controls.size(); ++i)
        if(m_controls[i]->wentDown())
            return true;
    return false;
}

bool GroupControl::wentUp() const
{
    for(size_t i = 0; i < m_controls.size(); ++i)
        if(m_controls[i]->wentUp())
            return true;
    return false;
}

void GroupControl::add(Control *ctrl)
{
    m_controls.push_back(ctrl);
}

}}
