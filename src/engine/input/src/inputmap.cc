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
#include    <input/inputmap.hh>
#include    <input/action.hh>
#include    <input/manager.hh>


namespace BugEngine { namespace Input
{

METACLASS_IMPL("Input",InputMap);
void InputMap::MetaClass::init(MetaClass* mc)
{
    super_t::init(mc);
    mc->m_manager.reset(new Manager);
}

const Manager* InputMap::MetaClass::getManager() const
{
    return m_manager.get();
}

InputMap::InputMap()
:   m_active(false)
{
}

InputMap::~InputMap()
{
}

Action* InputMap::addAction(ActionId id, const istring& name)
{
    if(id >= m_actions.size())
        m_actions.resize(id+1);
    Action* result = new Action(name, this); 
    m_actions[id] = result;
    return result;
}

void InputMap::mapControl(ActionId id, Control* button)
{
    m_actions[id]->mapControl(button);
}

bool InputMap::isDown(ActionId id) const
{
    return m_actions[id]->isDown();
}

bool InputMap::wentDown(ActionId id) const
{
    return m_actions[id]->wentDown();
}

bool InputMap::wentUp(ActionId id) const
{
    return m_actions[id]->wentUp();
}

float InputMap::value(ActionId id) const
{
    return m_actions[id]->value();
}

bool InputMap::active() const
{
    return m_active;
}

void InputMap::setActive(bool active)
{
    m_active = active;
}

}}
