/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>

namespace BugEngine
{

Environment& Environment::getEnvironment()
{
    static Environment s_environment;
    return s_environment;
}

}
