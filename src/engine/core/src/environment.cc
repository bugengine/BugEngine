/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
