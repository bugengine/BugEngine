/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/package.script.hh>

namespace BugEngine
{

Package::Package(weak<const File> file)
    :   Script(file)
{
}

Package::~Package()
{
}

}
