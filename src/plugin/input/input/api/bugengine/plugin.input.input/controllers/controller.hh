/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_CONTROLLER_HH_
#define BE_INPUT_CONTROLLERS_CONTROLLER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.input.input/stdafx.h>

namespace BugEngine { namespace Input {

class be_api(INPUT) Controller : public minitl::refcountable
{
    BE_NOCOPY(Controller);

protected:
    Controller();
    ~Controller();
};

}}  // namespace BugEngine::Input

/**************************************************************************************************/
#endif
