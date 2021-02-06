/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_MOUSE_HH_
#define BE_INPUT_CONTROLLERS_MOUSE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.input.input/stdafx.h>
#include <bugengine/plugin.input.input/controllers/controller.hh>

namespace BugEngine { namespace Input {

class Mouse : public Controller
{
    BE_NOCOPY(Mouse);

public:
    Mouse();
    ~Mouse();
};

}}  // namespace BugEngine::Input

/**************************************************************************************************/
#endif
