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
#include    <input/manager.hh>
#include    <devices/keyboard.hh>
#include    <devices/mouse.hh>
#include    <core/threads/event.hh>

#define WM_USER_SETMANAGER  WM_USER

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}

namespace BugEngine { namespace Input
{

class Manager::WindowCommunication
{
public:
    Event   m_event;
    HWND    m_hWnd;
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
    m_communication.m_event.wait();

    PostMessage(m_communication.m_hWnd, WM_USER_SETMANAGER, reinterpret_cast<WPARAM>(this), 0);

    unsigned int numDevices = 0;

    int kbIndex = 0;
    int mIndex = 0;
    int pasIndex = 0;

    BE_WIN32_CHECKRESULT(GetRawInputDeviceList(0, &numDevices, sizeof(RAWINPUTDEVICELIST)));
    RAWINPUTDEVICELIST* lst = (RAWINPUTDEVICELIST*)malloca(numDevices*sizeof(RAWINPUTDEVICELIST));
    BE_WIN32_CHECKRESULT(GetRawInputDeviceList(lst, &numDevices, sizeof(RAWINPUTDEVICELIST)));
    for(u32 i = 0; i < numDevices; ++i)
    {
        RID_DEVICE_INFO info;
        unsigned size = info.cbSize = sizeof(info);
        BE_WIN32_CHECKRESULT(GetRawInputDeviceInfoA( lst[i].hDevice, RIDI_DEVICEINFO, &info, &size));
        BE_WIN32_CHECKRESULT(GetRawInputDeviceInfoA( lst[i].hDevice, RIDI_DEVICENAME, 0, &size));
        char* deviceName = (char*)malloca(size+1);
        BE_WIN32_CHECKRESULT(GetRawInputDeviceInfoA( lst[i].hDevice, RIDI_DEVICENAME, deviceName, &size));
        deviceName[size] = 0;
        freea(deviceName);
        switch(info.dwType)
        {
        case RIM_TYPEKEYBOARD:
            OutputDebugString(minitl::format<>("keyboard %s :\ntype %d/subtype %d/keys / %d\n") | deviceName | (i32)info.keyboard.dwType | (i32)info.keyboard.dwSubType | (i32)info.keyboard.dwNumberOfKeysTotal);
            m_devices.insert(std::make_pair(lst[i].hDevice, new Keyboard(info.keyboard.dwNumberOfKeysTotal)));
            break;
        case RIM_TYPEMOUSE:
            m_devices.insert(std::make_pair(lst[i].hDevice, new Mouse(info.mouse.dwNumberOfButtons)));
            break;
        case RIM_TYPEHID:
            break;
        default:
            break;
        }
    }
    freea(lst);

    RAWINPUTDEVICE devices[2];
    devices[0].usUsagePage  = 0x01;
    devices[0].usUsage      = 0x02;
    devices[0].dwFlags      = RIDEV_INPUTSINK;
    devices[0].hwndTarget   = m_communication.m_hWnd;

    devices[1].usUsagePage  = 0x01;
    devices[1].usUsage      = 0x06;
    devices[1].dwFlags      = RIDEV_INPUTSINK;
    devices[1].hwndTarget   = m_communication.m_hWnd;

    RegisterRawInputDevices(devices, sizeof(devices)/sizeof(devices[0]), sizeof(RAWINPUTDEVICE));
}

Manager::~Manager()
{
    PostMessage(m_communication.m_hWnd, WM_QUIT, 0, 0);
    m_inputThread.wait();
}

Device* Manager::getDevice(void* handle)
{
    DeviceList::iterator it = m_devices.find(handle);
    if(it != m_devices.end())
        return it->second.get();
    else
        return 0;
}

void Manager::update() const
{
    for(DeviceList::const_iterator it = m_devices.begin(); it != m_devices.end(); ++it)
        it->second->update();
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static unsigned int nbWindows = 0;
    static int* memoryTest = 0;
    static Manager* m = 0;
    
    switch( msg )
    {
    case WM_USER_SETMANAGER:
        m = reinterpret_cast<Manager*>(wParam);
        return 0;
    case WM_INPUT:
        {
            UINT dwSize;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            RAWINPUT* rinput = (RAWINPUT*)malloca(dwSize);
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rinput, &dwSize, sizeof(RAWINPUTHEADER));
            switch(rinput->header.dwType)
            {
            case RIM_TYPEKEYBOARD:
                {
                    Device* d = m->getDevice(rinput->header.hDevice);
                    if(d)
                    {
                        float value = (rinput->data.keyboard.Message == WM_KEYDOWN||rinput->data.keyboard.Message == WM_SYSKEYDOWN) ? 1.0f : 0.0f;
                        checked_cast<Keyboard*>(d)->getBuffer()[rinput->data.keyboard.MakeCode] = value;
                    }
                }
                break;
            case RIM_TYPEMOUSE:
                break;
            default:;
            }
            freea(rinput);
        }
        break;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}


intptr_t Manager::waitInput(intptr_t p1, intptr_t p2)
{
    WindowCommunication& comm = WindowCommunication::getCommunication();
    WNDCLASSEX wndClassEx;
    const char className[] = "__BugEngine__input__";

    memset(&wndClassEx, 0, sizeof(WNDCLASSEX));
    wndClassEx.lpszClassName  = className;
    wndClassEx.cbSize         = sizeof(WNDCLASSEX);
    wndClassEx.style          = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;
    wndClassEx.lpfnWndProc    = WindowProc;
    wndClassEx.hInstance      = hDllInstance;
    wndClassEx.hIcon          = 0;
    wndClassEx.hIconSm        = 0;
    wndClassEx.hCursor        = LoadCursor(0, (LPCTSTR)IDC_ARROW);
    wndClassEx.hbrBackground  = NULL;
    wndClassEx.lpszMenuName   = NULL;
    wndClassEx.cbClsExtra     = 0;
    wndClassEx.cbWndExtra     = 0;
    RegisterClassEx(&wndClassEx);

    comm.m_hWnd = CreateWindowEx( 0,
                                  className,
                                  className,
                                  0,
                                  0, 0,
                                  1, 1,
                                  NULL, NULL, hDllInstance, NULL );
    comm.m_event.set();

    MSG msg;
    while(GetMessage(&msg, 0, 0, 0))
    {
        switch(msg.message)
        {
        case WM_QUIT:
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DestroyWindow(comm.m_hWnd);
    comm.m_hWnd = 0;
    UnregisterClass(className, hDllInstance);
    return 0;
}

}}
