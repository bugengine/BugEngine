/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <package.script.hh>

namespace BugEngine { namespace PackageBuilder
{

Package::Package(ref<const File> file)
    :   Script(file)
{
}

Package::~Package()
{
}

}}
