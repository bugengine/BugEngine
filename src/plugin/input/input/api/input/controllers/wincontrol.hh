/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROLLERS_WINCONTROL_HH_
#define BE_INPUT_CONTROLLERS_WINCONTROL_HH_
/*****************************************************************************/
#include    <input/controllers/controller.hh>


namespace BugEngine { namespace Input
{

class Wincontrol : public Controller
{
    BE_NOCOPY(Wincontrol);
public:
    Wincontrol();
    ~Wincontrol();
};

}}

/*****************************************************************************/
#endif
