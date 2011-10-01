/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/folder.script.hh>
#include    <system/file/file.script.hh>

#include    <minitl/container/algorithm.hh>

#include    <core/threads/thread.hh>
#include    <core/threads/semaphore.hh>

namespace BugEngine
{

struct IOContext
{
    IOContext();
    ~IOContext();
    static intptr_t ioProcess(intptr_t p1, intptr_t p2);
};


static bool         s_ioDone;
static Semaphore    s_ioSemaphore(0, 32768);
static Thread       s_ioThread("IOThread", &IOContext::ioProcess, 0, 0, Thread::Highest);
static IOContext    s_iocontext;

IOContext::IOContext()
{
};

IOContext::~IOContext()
{
    s_ioDone = true;
    s_ioSemaphore.release(1);
}

intptr_t IOContext::ioProcess(intptr_t p1, intptr_t p2)
{
    while(s_ioSemaphore.wait())
    {
        if (s_ioDone)
            break;
        /* doWork*/
    }
    return 0;
}



Folder::Folder()
    :   m_files(fsArena())
    ,   m_folders(fsArena())
    ,   m_mounts(fsArena())
{
}

Folder::~Folder()
{
}

weak<File> Folder::openFile(istring name)
{
    for (minitl::vector< minitl::pair< istring, ref<File> > >::const_iterator it = m_files.begin(); it != m_files.end(); ++it)
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
    for (minitl::vector< minitl::pair< istring, ref<Folder> > >::const_iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    for (minitl::vector< minitl::pair< istring, ref<Folder> > >::const_iterator it = m_folders.begin(); it != m_folders.end(); ++it)
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
    for (minitl::vector< minitl::pair< istring, ref<Folder> > >::const_iterator it = m_folders.begin(); it != m_folders.end(); ++it)
    {
        if (it->first == name)
        {
            be_warning("mounting filesystem hides folder %s" | name);
        }
    }
    for (minitl::vector< minitl::pair< istring, ref<Folder> > >::iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
    {
        if (it->first == name)
        {
            be_warning("mounting filesystem will unmount filesystem %s" | name);
            it->second = folder;
            return;
        }
    }
    m_mounts.push_back(minitl::make_pair(name, folder));
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
    for (minitl::vector< minitl::pair< istring, ref<Folder> > >::iterator it = m_mounts.begin(); it != m_mounts.end(); ++it)
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



}
