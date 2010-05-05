/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_DEVICE_MOUSE_HH_
#define BE_INPUT_DEVICE_MOUSE_HH_
/*****************************************************************************/
#include    <input/device.hh>

namespace BugEngine { namespace Input
{

class be_api(INPUT) Mouse : public Device
{
private:
    static size_t   s_miceCount;
public:
    Mouse(size_t numButtons);
    ~Mouse();
};

}}

/*****************************************************************************/
#endif
