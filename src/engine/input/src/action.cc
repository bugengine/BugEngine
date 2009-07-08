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
#include    <input/action.hh>
#include    <input/control.hh>
#include    <input/inputmap.hh>


namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",Action);

Action::Action(const istring& name, const InputMap* owner)
:   m_name(name)
,   m_owner(owner)
{
}

Action::~Action()
{
}

const istring& Action::name() const
{
    return m_name;
}

float Action::value() const
{
    float value = 0.0;
    if(m_owner->active()) for(size_t i = 0; i < m_controls.size(); ++i)
    {
        if(m_controls[i]->value() > value)
            value = m_controls[i]->value();
    }
    return value;
}

bool Action::isDown() const
{
    if(m_owner->active()) for(size_t i = 0; i < m_controls.size(); ++i)
    {
        if(m_controls[i]->isDown())
            return true;
    }
    return false;
}

bool Action::wentDown() const
{
    if(m_owner->active()) for(size_t i = 0; i < m_controls.size(); ++i)
    {
        if(m_controls[i]->wentDown())
            return true;
    }
    return false;
}

bool Action::wentUp() const
{
    if(m_owner->active()) for(size_t i = 0; i < m_controls.size(); ++i)
    {
        if(m_controls[i]->wentUp())
            return true;
    }
    return false;
}

void Action::mapControl(Control* c)
{
    m_controls.push_back(c);
}

void Action::unmapControl(Control* c)
{
    for(std::vector<Control*>::iterator it = m_controls.begin(); it != m_controls.end(); )
    {
        if(*it == c)
        {
            it = m_controls.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

}}
