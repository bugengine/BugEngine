/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packageloader.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace PackageManager
{

PackageLoader::PackageLoader()
{
}

PackageLoader::~PackageLoader()
{
}

void PackageLoader::loadPackage(weak<const Package> package)
{
    struct PackageHeader
    {
        int version;
    };
    Allocator::Block<u8> buffer(tempArena(), sizeof(PackageHeader));
    ref<const File::Ticket> ticket = package->m_packageFile->beginRead(0, sizeof(PackageHeader));
    while(!ticket->done())
        /* wait */
        Thread::yield();
}

void PackageLoader::unloadPackage(const ResourceHandle& /*handle*/)
{
}

}}
