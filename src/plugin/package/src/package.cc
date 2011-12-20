/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/package.script.hh>

namespace BugEngine { namespace PackageManager
{

Package::Package(ref<const File> file)
    :   Script(file)
{
}

Package::~Package()
{
}

}}
