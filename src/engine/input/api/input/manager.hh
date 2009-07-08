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

#ifndef BE_INPUT_MANAGER_HH_
#define BE_INPUT_MANAGER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <input/device.hh>

namespace BugEngine { namespace Input
{

class Manager
{
private:
    class WindowCommunication;
    typedef std::map< void*, refptr<Device> >   DeviceList;
private:
    WindowCommunication&    m_communication;
    Thread                  m_inputThread;
    DeviceList              m_devices;
private:
    static intptr_t waitInput(intptr_t p1, intptr_t p2);
public:
    Manager();
    ~Manager();

    Device* getDevice(void* handle);

    void update() const;
private:
    Manager& operator=(const Manager& other);
    Manager(const Manager& other);
};

}}

/*****************************************************************************/
#endif
