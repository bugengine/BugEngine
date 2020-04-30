/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_WINCONTROL_HH_
#define BE_INPUT_CONTROLLERS_WINCONTROL_HH_
/**************************************************************************************************/
#include <bugengine/plugin.input.input/stdafx.h>
#include <bugengine/plugin.input.input/controllers/controller.hh>

namespace BugEngine { namespace Input {

class Wincontrol : public Controller
{
    BE_NOCOPY(Wincontrol);

public:
    Wincontrol();
    ~Wincontrol();
};

}}  // namespace BugEngine::Input

/**************************************************************************************************/
#endif
