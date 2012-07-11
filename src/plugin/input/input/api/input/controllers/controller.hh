/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
