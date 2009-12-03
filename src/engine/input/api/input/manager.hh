/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_MANAGER_HH_
#define BE_INPUT_MANAGER_HH_
/*****************************************************************************/
#include    <core/threads/thread.hh>
#include    <input/device.hh>

namespace BugEngine { namespace Input
{

class be_api(INPUT) Manager
{
private:
    class WindowCommunication;
    typedef std::map< void*, ref<Device> >   DeviceList;
private:
    WindowCommunication&    m_communication;
    Thread                  m_inputThread;
    DeviceList              m_devices;
private:
    static intptr_t waitInput(intptr_t p1, intptr_t p2);
public:
    Manager();
    ~Manager();

    weak<Device> getDevice(void* handle);

    void update() const;
private:
    Manager& operator=(const Manager& other);
    Manager(const Manager& other);
};

}}

/*****************************************************************************/
#endif
