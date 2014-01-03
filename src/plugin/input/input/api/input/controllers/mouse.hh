/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_MOUSE_HH_
#define BE_INPUT_CONTROLLERS_MOUSE_HH_
/*****************************************************************************/
#include    <input/controllers/controller.hh>


namespace BugEngine { namespace Input
{

class Mouse : public Controller
{
    BE_NOCOPY(Mouse);
public:
    Mouse();
    ~Mouse();
};

}}

/*****************************************************************************/
#endif
