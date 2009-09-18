/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <groupcontrol.hh>
#include    <input/device.hh>


namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",GroupControl);

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
