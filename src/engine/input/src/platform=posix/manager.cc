/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/manager.hh>


namespace BugEngine { namespace Input
{


class Manager::WindowCommunication
{
public:
    WindowCommunication();
    ~WindowCommunication();

    static WindowCommunication& getCommunication();
};

Manager::WindowCommunication::WindowCommunication()
{
}

Manager::WindowCommunication::~WindowCommunication()
{
}

Manager::WindowCommunication& Manager::WindowCommunication::getCommunication()
{
    static WindowCommunication  s_communication;
    return s_communication;
}


Manager::Manager()
:   m_communication(WindowCommunication::getCommunication())
,   m_inputThread("Input", &Manager::waitInput, 0, 0, Thread::Critical, false)
{
}

Manager::~Manager()
{
}

weak<Device> Manager::getDevice(void* handle)
{
    return weak<Device>();
}

void Manager::update() const
{
}

intptr_t Manager::waitInput(intptr_t /*p1*/, intptr_t /*p2*/)
{
    return 0;
}

}}

