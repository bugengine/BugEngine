/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_DEVICECONTROL_HH_
#define BE_INPUT_DEVICECONTROL_HH_
/*****************************************************************************/
#include    <input/control.hh>

namespace BugEngine { namespace Input
{

class Device;

class DeviceControl : public Control
{
private:
    const Device*   m_device;
    size_t          m_index;
public:
    DeviceControl(const istring& name,const  Device* device, size_t index);
    ~DeviceControl();

    float           value() const override;
    bool            isDown() const override;
    bool            wasDown() const override;
    bool            wentDown() const override;
    bool            wentUp() const override;
    size_t          index() const { return m_index; }

    be_metaclass(NONE,DeviceControl,Control)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
