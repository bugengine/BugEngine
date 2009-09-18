/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_FRAME_HH_
#define BE_MAIN_FRAME_HH_
/*****************************************************************************/
#include <system/scheduler/task.hh>

namespace BugEngine
{

class Frame : public Object
{
public:
    Frame();
    ~Frame();

    int run();

    be_metaclass(NOEXPORT,Frame, Object)
    be_properties
    be_end
};

}

/*****************************************************************************/
#endif
