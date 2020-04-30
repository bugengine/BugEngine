/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <ioprocess.hh>

#include <bugengine/core/threads/thread.hh>

namespace BugEngine {

Folder::Folder()
    : m_lock()
    , m_files(Arena::filesystem())
    , m_folders(Arena::filesystem())
    , m_mounts(Arena::filesystem())
    , m_upToDate(false)
{
    IOProcess::IOContext::begin();
}

Folder::~Folder()
{
    IOProcess::IOContext::end();
}

weak< File > Folder::openFile(istring name)
{
    ScopedCriticalSection lock(m_lock);
    return openFileNoLock(name);
}

weak< File > Folder::openFile(ifilename name)
{
    ScopedCriticalSection lock(m_lock);
    return openFileNoLock(name);
}

weak< Folder > Folder::openFolder(istring name)
{
    ScopedCriticalSection lock(m_lock);
    return openFolderNoLock(name);
}

weak< Folder > Folder::openFolder(ipath name)
{
    ScopedCriticalSection lock(m_lock);
    return openFolderNoLock(name);
}

weak< File > Folder::openFileNoLock(istring name)
{
    for(minitl::vector< minitl::tuple< istring, ref< File > > >::const_iterator it = m_files.begin();
        it != m_files.end(); ++it)
    {
        if(it->first == name) return it->second;
    }
    return weak< File >();
}

weak< File > Folder::openFileNoLock(ifilename name)
{
    istring s = name.pop_front();
    if(name.size() > 0)
    {
        weak< Folder > f = openFolderNoLock(s);
        if(f)
        {
            return f->openFileNoLock(name);
        }
        else
        {
            return weak< File >();
        }
    }
    else
    {
        return openFileNoLock(s);
    }
}

weak< Folder > Folder::openFolderNoLock(istring name)
{
    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::const_iterator it = m_mounts.begin();
        it != m_mounts.end(); ++it)
    {
        if(it->first == name) return it->second;
    }
    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::const_iterator it = m_folders.begin();
        it != m_folders.end(); ++it)
    {
        if(it->first == name) return it->second;
    }
    return weak< Folder >();
}

weak< Folder > Folder::openFolderNoLock(ipath name)
{
    istring s = name.pop_front();
    if(name.size() > 0)
    {
        weak< Folder > f = openFolderNoLock(s);
        if(f)
        {
            return f->openFolderNoLock(name);
        }
        else
        {
            return weak< Folder >();
        }
    }
    else
    {
        return openFolderNoLock(s);
    }
}

void Folder::mount(istring name, ref< Folder > folder)
{
    ScopedCriticalSection lock(m_lock);
    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::const_iterator it = m_folders.begin();
        it != m_folders.end(); ++it)
    {
        if(it->first == name)
        {
            be_warning("mounting filesystem hides folder %s" | name);
        }
    }
    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it = m_mounts.begin(); it != m_mounts.end();
        ++it)
    {
        if(it->first == name)
        {
            be_warning("mounting filesystem will unmount filesystem %s" | name);
            it->second = folder;
            return;
        }
    }
    m_mounts.push_back(minitl::make_tuple(name, folder));
}

void Folder::mount(ipath name, ref< Folder > folder)
{
    istring s = name.pop_front();
    if(name.size() > 0)
    {
        weak< Folder > f = openFolder(s);
        if(f)
        {
            f->mount(name, folder);
        }
        else
        {
            be_error("could not mount folder; path %s does not exist in the file system" | name);
        }
    }
    else
    {
        mount(s, folder);
    }
}

void Folder::umount(istring name)
{
    ScopedCriticalSection lock(m_lock);
    for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it = m_mounts.begin(); it != m_mounts.end();
        ++it)
    {
        if(it->first == name)
        {
            minitl::swap(*it, m_mounts.back());
            m_mounts.pop_back();
            return;
        }
    }
    be_error("could not unmount folder; path %s is not mounted" | name);
}

void Folder::umount(ipath name)
{
    istring s = name.pop_front();
    if(name.size() > 0)
    {
        weak< Folder > f = openFolder(s);
        if(f)
        {
            f->umount(name);
        }
        else
        {
            be_error("could not unmount folder; path %s does not exist in the file system" | name);
        }
    }
    else
    {
        umount(s);
    }
}

void Folder::refresh(Folder::ScanPolicy scanPolicy)
{
    if(!m_upToDate)
    {
        doRefresh(scanPolicy);
        m_upToDate = true;
    }
}

void Folder::doRefresh(ScanPolicy scanPolicy)
{
    if(scanPolicy == Folder::ScanRecursive)
    {
        ScopedCriticalSection lock(m_lock);
        for(minitl::vector< minitl::tuple< istring, ref< Folder > > >::iterator it = m_folders.begin();
            it != m_folders.end(); ++it)
        {
            it->second->refresh(scanPolicy);
        }
    }
}

}  // namespace BugEngine
