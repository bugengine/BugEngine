/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>

namespace BugEngine { namespace PackageManager
{

PackageLoader::PackageLoader(weak<const FileSystem> filesystem)
    :   m_filesystem(filesystem)
{
}

PackageLoader::~PackageLoader()
{
}

}}
