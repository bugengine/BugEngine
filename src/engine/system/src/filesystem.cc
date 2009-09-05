/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/
    
#include    <system/stdafx.h>
#include    <system/filesystem.hh>

#include    <core/memory/streams.hh>
#include    <system/fscomponent.hh>


namespace BugEngine
{

class FileSystem::FileSystemMountPoint
{
public:
    typedef std::map<istring, FileSystemMountPoint*> ChildrenMap;
private:
    refptr<const FileSystemComponent>   m_component;
    ChildrenMap                         m_children;
public:
    FileSystemMountPoint(const FileSystemComponent *component = 0);
    ~FileSystemMountPoint();

    FileSystemMountPoint* getOrCreate(const istring& child);
    FileSystemMountPoint* get(const istring& child);
    void erase(const istring& child);

    void mount(const refptr<const FileSystemComponent>& component);
    void umount();
    const FileSystemComponent*  component() const;

    bool empty() const;
};

FileSystem::FileSystemMountPoint::FileSystemMountPoint(const FileSystemComponent *component) :
    m_component(component)
{
}
    
FileSystem::FileSystemMountPoint::~FileSystemMountPoint()
{
    for(ChildrenMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
        delete it->second;
}

FileSystem::FileSystemMountPoint* FileSystem::FileSystemMountPoint::getOrCreate(const istring& child)
{
    std::pair<ChildrenMap::iterator, bool> result = m_children.insert(std::make_pair(child, (FileSystemMountPoint*)0));
    if(result.second)
        result.first->second = new FileSystemMountPoint;
    return result.first->second;
}

FileSystem::FileSystemMountPoint* FileSystem::FileSystemMountPoint::get(const istring& child)
{
    ChildrenMap::iterator it = m_children.find(child);
    if(it != m_children.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

void FileSystem::FileSystemMountPoint::erase(const istring& child)
{
    ChildrenMap::iterator it = m_children.find(child);
    be_assert(it != m_children.end(), "no mount point %s in filesystem" | child.c_str());
    delete it->second;
    m_children.erase(it);
}

void FileSystem::FileSystemMountPoint::mount(const refptr<const FileSystemComponent> &component)
{
    be_assert(m_component == 0, "cannot mount null component");
    m_component = component;
}

void FileSystem::FileSystemMountPoint::umount()
{
    be_assert(m_component != 0, "cannot unmount null component");
    m_component = 0;
}

const FileSystemComponent* FileSystem::FileSystemMountPoint::component() const
{
    return m_component.get();
}

bool FileSystem::FileSystemMountPoint::empty() const
{
    return m_component == 0 && m_children.empty();
}

//-----------------------------------------------------------------------------

FileSystem::FileSystem(void) :
    m_root(new FileSystemMountPoint(0))
{
}

FileSystem::~FileSystem(void)
{
    delete m_root;
}

void FileSystem::mount(const ipath& prefix, refptr<const FileSystemComponent> component)
{
    FileSystemMountPoint* mountPoint = m_root;
    for(size_t i = 0; i < prefix.size(); ++i)
    {
        mountPoint = mountPoint->getOrCreate(prefix[i]);
    }
    be_assert(mountPoint->component() == 0, "component %s already mounted" | prefix.str().c_str());
    mountPoint->mount(component);
}

void FileSystem::umount(const ipath& prefix)
{
    FileSystemMountPoint* mountPoint = m_root;
    FileSystemMountPoint* parent     = 0;
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

refptr<AbstractMemoryStream> FileSystem::open(const ifilename& file, FileOpenMode mode) const
{
    FileSystemMountPoint* mountPoint = m_root;
    FileSystemMountPoint* bestmatch = mountPoint;
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

}
