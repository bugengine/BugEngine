/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>

namespace BugEngine { namespace PackageManager
{

PackageLoader::PackageLoader(weak<const Folder> dataFolder)
    :   m_dataFolder(dataFolder)
{
}

PackageLoader::~PackageLoader()
{
}

}}
