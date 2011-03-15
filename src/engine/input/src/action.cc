/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/action.hh>
#include    <input/control.hh>
#include    <input/inputmap.hh>


namespace BugEngine { namespace Input
{

Action::Action(const istring& name, const InputMap* owner)
:   m_name(name)
,   m_owner(owner)
,   m_controls(inputArena())
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
    if (m_owner->active()) for (size_t i = 0; i < m_controls.size(); ++i)
    {
        if (m_controls[i]->value() > value)
            value = m_controls[i]->value();
    }
    return value;
}

bool Action::isDown() const
{
    if (m_owner->active()) for (size_t i = 0; i < m_controls.size(); ++i)
    {
        if (m_controls[i]->isDown())
            return true;
    }
    return false;
}

bool Action::wentDown() const
{
    if (m_owner->active()) for (size_t i = 0; i < m_controls.size(); ++i)
    {
        if (m_controls[i]->wentDown())
            return true;
    }
    return false;
}

bool Action::wentUp() const
{
    if (m_owner->active()) for (size_t i = 0; i < m_controls.size(); ++i)
    {
        if (m_controls[i]->wentUp())
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
    for (minitl::vector<Control*>::iterator it = m_controls.begin(); it != m_controls.end(); )
    {
        if (*it == c)
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
