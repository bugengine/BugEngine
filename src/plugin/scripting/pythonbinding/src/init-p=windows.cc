/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <core/environment.hh>

namespace BugEngine { namespace Python
{

void platformInit()
{
    be_info(Py_GetPath());
}

void platformSetup()
{
}

}}
