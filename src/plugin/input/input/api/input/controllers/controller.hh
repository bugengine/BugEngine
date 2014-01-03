/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_CONTROLLER_HH_
#define BE_INPUT_CONTROLLERS_CONTROLLER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Input
{

class be_api(INPUT) Controller : public minitl::refcountable
{
    BE_NOCOPY(Controller);
protected:
    Controller();
    ~Controller();
};

}}

/*****************************************************************************/
#endif
