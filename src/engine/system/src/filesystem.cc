/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/filesystem.hh>

#include    <core/memory/streams.hh>
#include    <system/fscomponent.hh>


namespace BugEngine
{

FileSystem::FileSystemMountPoint::FileSystemMountPoint(ref<const FileSystemComponent> component)
:   m_component(component)
{
}

FileSystem::FileSystemMountPoint::~FileSystemMountPoint()
{
    for(ChildrenMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
        delete it->second;
}

weak<FileSystem::FileSystemMountPoint> FileSystem::FileSystemMountPoint::getOrCreate(const istring& child)
{
    std::pair<ChildrenMap::iterator, bool> result = m_children.insert(std::make_pair(child, scoped<FileSystemMountPoint>()));
    if(result.second)
        result.first->second = scoped<FileSystemMountPoint>::create();
    return result.first->second;
}

weak<FileSystem::FileSystemMountPoint> FileSystem::FileSystemMountPoint::get(const istring& child)
{
    ChildrenMap::iterator it = m_children.find(child);
    if(it != m_children.end())
    {
        return it->second;
    }
    else
    {
        return weak<FileSystem::FileSystemMountPoint>();
    }
}

void FileSystem::FileSystemMountPoint::erase(const istring& child)
{
    ChildrenMap::iterator it = m_children.find(child);
    be_assert(it != m_children.end(), "no mount point %s in filesystem" | child.c_str());
    delete it->second;
    m_children.erase(it);
}

void FileSystem::FileSystemMountPoint::mount(const ref<const FileSystemComponent> &component)
{
    be_assert(m_component == 0, "cannot mount null component");
    m_component = component;
}

void FileSystem::FileSystemMountPoint::umount()
{
    be_assert(m_component != 0, "cannot unmount null component");
    m_component = ref<const FileSystemComponent>();
}

weak<const FileSystemComponent> FileSystem::FileSystemMountPoint::component() const
{
    return m_component;
}

bool FileSystem::FileSystemMountPoint::empty() const
{
    return m_component == 0 && m_children.empty();
}

//-----------------------------------------------------------------------------

FileSystem::FileSystem(void)
: m_root(scoped<FileSystemMountPoint>::create())
{
}

FileSystem::~FileSystem(void)
{
}

void FileSystem::mount(const ipath& prefix, ref<const FileSystemComponent> component)
{
    weak<FileSystemMountPoint> mountPoint = m_root;
    for(size_t i = 0; i < prefix.size(); ++i)
    {
        mountPoint = mountPoint->getOrCreate(prefix[i]);
    }
    be_assert(mountPoint->component() == 0, "component %s already mounted" | prefix.str().c_str());
    mountPoint->mount(component);
}

void FileSystem::umount(const ipath& prefix)
{
    weak<FileSystemMountPoint> mountPoint = m_root;
    weak<FileSystemMountPoint> parent     = 0;
    for(size_t i = 0; i < prefix.size(); ++i)
    {
        parent = mountPoint;
        mountPoint = mountPoint->get(prefix[i]);
        be_assert(mountPoint, "cannot find filesystem component %s in %s" | prefix[i].c_str() | prefix.str().c_str());
    }

    mountPoint->umount();
    if(parent && mountPoint->empty())
        parent->erase(prefix[prefix.size()-1]);
}

ref<AbstractMemoryStream> FileSystem::open(const ifilename& file, FileOpenMode mode) const
{
    weak<FileSystemMountPoint> mountPoint = m_root;
    weak<FileSystemMountPoint> bestmatch = mountPoint;
    ifilename suffix = file;
    for(size_t i = 0; (i < file.size()-1) && mountPoint; ++i)
    {
        bestmatch = mountPoint;
        mountPoint = mountPoint->get(suffix[0]);
        if(mountPoint)
            suffix.pop_front();
    }
    be_assert(bestmatch, "cannot open file %s" | file.str().c_str());
    return bestmatch->component()->open(suffix, mode);
}

size_t FileSystem::age(const ifilename& /*file*/) const
{
    be_unimplemented();
    return 0;
}

std::set<ifilename> FileSystem::listFiles(const ipath& /*prefix*/, const char* /*extension*/)
{
    be_unimplemented();
    return std::set<ifilename>();
}

std::set<ipath> FileSystem::listDirectories(const ipath& /*prefix*/)
{
    be_unimplemented();
    return std::set<ipath>();
}

weak<FileSystem> FileSystem::instance()
{
    static scoped<FileSystem> s_filesystem(scoped<FileSystem>::create());
    return s_filesystem;
}

}
