/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>
#include    <core/threads/thread.hh>

namespace BugEngine
{

Allocator& packageArena()
{
    return gameArena();
}

}

namespace BugEngine { namespace PackageManager
{

Allocator& packageArena()
{
    return gameArena();
}

PackageLoader::PackageLoader()
    :   ScriptEngine<Package>(packageArena())
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::runBuffer(weak<const Package> script, const Allocator::Block<u8>& buffer)
{
    struct PackageHeader
    {
        int version;
    };
}

}}
