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
#include    <system/diskfs.hh>
#include    <core/memory/streams.hh>


namespace BugEngine
{

class MemoryFileMap : public AbstractMemoryStream
{
private:
    HANDLE  m_file;
    HANDLE  m_fileMap;
    void*   m_pointer;
    i64     m_size;
    i64     m_offset;
public:
    MemoryFileMap(void* memory, size_t size, HANDLE file, HANDLE filemap);
    ~MemoryFileMap();

    virtual void* basememory() override;
    virtual i64   size() const override;
    virtual i64   offset() const override;
    virtual void  seek(SeekMethod method, i64 offset) override;
    virtual void  resize(i64 size) override;
    virtual bool  writable() const override;
};

MemoryFileMap::MemoryFileMap(void* memory, size_t size, HANDLE file, HANDLE filemap) :
    m_file(file),
    m_fileMap(filemap),
    m_pointer(memory),
    m_size(size),
    m_offset(0)
{
}

MemoryFileMap::~MemoryFileMap()
{
    UnmapViewOfFile(m_pointer);
    CloseHandle(m_fileMap);
    CloseHandle(m_file);
}

void* MemoryFileMap::basememory()
{
    return m_pointer;
}

i64 MemoryFileMap::size() const
{
    return m_size;
}

i64 MemoryFileMap::offset() const
{
    return m_offset;
}

void MemoryFileMap::seek(SeekMethod method, i64 _offset)
{
    switch(method)
    {
        case eSeekMove:
            m_offset += _offset;
            break;
        case eSeekFromStart:
            m_offset = _offset;
            break;
        case eSeekFromEnd:
            m_offset = m_size + _offset;
            break;
        default:
            AssertNotReached();
    }
    if(m_offset < 0) m_offset = 0;
    if(m_offset > m_size) m_offset = m_size;
}

void MemoryFileMap::resize(i64 size)
{
    UNUSED(size);
    AssertNotReached();
}

bool MemoryFileMap::writable() const
{
    return false;
}

//-----------------------------------------------------------------------------

DiskFS::DiskFS(const ipath& prefix, bool readonly) :
    FileSystemComponent(),
    m_prefix(prefix),
    m_readOnly(readonly)
{
}

DiskFS::~DiskFS(void)
{
}

bool DiskFS::writable() const
{
    return m_readOnly;
}

refptr<AbstractMemoryStream> DiskFS::open(const ifilename& filename, FileOpenMode mode) const
{
    std::string fullname = (m_prefix+filename).str();
    if(mode == eReadOnly)
    {
        HANDLE file = CreateFile(fullname.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if(file == INVALID_HANDLE_VALUE)
        {
            DWORD err = GetLastError();
            throw EFileNotFound(filename);
        }
        else
        {
            DWORD sizehigh;
            HANDLE filemap = CreateFileMapping(file, 0, PAGE_READONLY, 0, 0, (m_prefix+filename).tostring("/").c_str());
            LPVOID memory = MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);
            return new MemoryFileMap(memory, GetFileSize(file, &sizehigh), file, filemap);
        }
    }
    else
        throw EFileNotFound(filename);
}

size_t DiskFS::age(const ifilename& file) const
{
    std::string name = (m_prefix+file).str();

    return 0;
}

}
