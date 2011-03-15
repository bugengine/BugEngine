/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/inputmap.hh>
#include    <input/action.hh>
#include    <input/manager.hh>


namespace BugEngine { namespace Input
{

InputMap::InputMap()
:   m_actions(inputArena())
,   m_active(false)
{
}

InputMap::~InputMap()
{
}

weak<Action> InputMap::addAction(ActionId id, const istring& name)
{
    if (id >= m_actions.size())
        m_actions.resize(id+1);
    ref<Action> result = ref<Action>::create(inputArena(), name, this); 
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
