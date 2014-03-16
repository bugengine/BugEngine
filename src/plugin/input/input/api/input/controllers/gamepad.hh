/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_GAMEPAD_HH_
#define BE_INPUT_CONTROLLERS_GAMEPAD_HH_
/**************************************************************************************************/
#include    <input/stdafx.h>
#include    <input/controllers/controller.hh>

namespace BugEngine { namespace Input
{

class Gamepad : public Controller
{
    BE_NOCOPY(Gamepad);
public:
    Gamepad();
    ~Gamepad();
};

}}

/**************************************************************************************************/
#endif
