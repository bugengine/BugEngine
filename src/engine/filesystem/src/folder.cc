/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <filesystem/folder.script.hh>
#include    <filesystem/file.script.hh>

#include    <core/threads/thread.hh>

namespace BugEngine
{


Folder::Folder()
    :   m_lock()
    ,   m_files(Arena::filesystem())
    ,   m_folders(Arena::filesystem())
    ,   m_mounts(Arena::filesystem())
    ,   m_upToDate(false)
{
}

Folder::~Folder()
{
}

weak<File> Folder::openFile(istring name)
{
    ScopedCriticalSection lock(m_lock);
    for (minitl::vector< minitl::tuple< istring, ref<File> > >::const_iterator it = m_files.begin(); it != m_files.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    return weak<File>();
}

weak<File> Folder::openFile(ifilename name)
{
    istring s = name.pop_front();
    if (name.size() > 1)
    {
        weak<Folder> f = openFolder(s);
        if (f)
        {
            return f->openFile(name);
        }
        else
        {
            return weak<File>();
        }
    }
    else
    {
        return openFile(s);
    }
}

weak<Folder> Folder::openFolder(istring name)
{
    ScopedCriticalSection lock(m_lock);
    for (minitl::vector< minitl::tuple< istring, ref<Folder> > >::const_iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    for (minitl::vector< minitl::tuple< istring, ref<Folder> > >::const_iterator it = m_folders.begin(); it != m_folders.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    return weak<Folder>();
}

weak<Folder> Folder::openFolder(ipath name)
{
    istring s = name.pop_front();
    if (name.size() > 0)
    {
        weak<Folder> f = openFolder(s);
        if (f)
        {
            return f->openFolder(name);
        }
        else
        {
            return weak<Folder>();
        }
    }
    else
    {
        return openFolder(s);
    }
}

void Folder::mount(istring name, ref<Folder> folder)
{
    ScopedCriticalSection lock(m_lock);
    for (minitl::vector< minitl::tuple< istring, ref<Folder> > >::const_iterator it = m_folders.begin(); it != m_folders.end(); ++it)
    {
        if (it->first == name)
        {
            be_warning("mounting filesystem hides folder %s" | name);
        }
    }
    for (minitl::vector< minitl::tuple< istring, ref<Folder> > >::iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
    {
        if (it->first == name)
        {
            be_warning("mounting filesystem will unmount filesystem %s" | name);
            it->second = folder;
            return;
        }
    }
    m_mounts.push_back(minitl::make_tuple(name, folder));
}

void Folder::mount(ipath name, ref<Folder> folder)
{
    istring s = name.pop_front();
    if (name.size() > 0)
    {
        weak<Folder> f = openFolder(s);
        if (f)
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
    for (minitl::vector< minitl::tuple< istring, ref<Folder> > >::iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
    {
        if (it->first == name)
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
    if (name.size() > 0)
    {
        weak<Folder> f = openFolder(s);
        if (f)
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
    if (!m_upToDate)
    {
        doRefresh(scanPolicy);
        m_upToDate = true;
    }
}

void Folder::doRefresh(ScanPolicy scanPolicy)
{
    if (scanPolicy == Folder::ScanRecursive)
    {
        ScopedCriticalSection lock(m_lock);
        for (minitl::vector< minitl::tuple<istring, ref<Folder> > >::iterator it = m_folders.begin(); it != m_folders.end(); ++it)
        {
            it->second->refresh(scanPolicy);
        }
    }

}

}
