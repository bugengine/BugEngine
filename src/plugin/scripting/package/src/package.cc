/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
