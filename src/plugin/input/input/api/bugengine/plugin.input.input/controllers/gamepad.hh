/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_GAMEPAD_HH_
#define BE_INPUT_CONTROLLERS_GAMEPAD_HH_
/**************************************************************************************************/
#include <bugengine/plugin.input.input/stdafx.h>
#include <bugengine/plugin.input.input/controllers/controller.hh>

namespace BugEngine { namespace Input {

class Gamepad : public Controller
{
    BE_NOCOPY(Gamepad);

public:
    Gamepad();
    ~Gamepad();
};

}}  // namespace BugEngine::Input

/**************************************************************************************************/
#endif
