/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/diskfs.hh>
#include    <core/memory/streams.hh>

#include    <unistd.h>
#include    <sys/mman.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <fcntl.h>

namespace BugEngine
{

class MemoryFileMap : public IMemoryStream
{
private:
    const int       m_file;
    void* const     m_pointer;
    const i64       m_size;
    i64             m_offset;
public:
    MemoryFileMap(int file, off_t size);
    ~MemoryFileMap();

    virtual void*       basememory() override;
    virtual const void* basememory() const override;
    virtual i64         size() const override;
    virtual i64         offset() const override;
    virtual void        seek(SeekMethod method, i64 offset);
    virtual void        resize(i64 size) override;
    virtual bool        writable() const override;
};

MemoryFileMap::MemoryFileMap(int file, off_t size) :
    m_file(file),
    m_pointer(mmap(0, size, PROT_READ, MAP_SHARED, file, 0)),
    m_size(size),
    m_offset(0)
{
}

MemoryFileMap::~MemoryFileMap()
{
    munmap(m_pointer,m_size);
    close(m_file);
}

void* MemoryFileMap::basememory()
{
    return m_pointer;
}

const void* MemoryFileMap::basememory() const
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
            be_notreached();
    }
    if (m_offset < 0) m_offset = 0;
    if (m_offset > m_size) m_offset = m_size;
}

void MemoryFileMap::resize(i64 size)
{
    be_forceuse(size);
    be_unimplemented();
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

ref<IMemoryStream> DiskFS::open(const ifilename& filename, FileOpenMode mode) const
{
    minitl::format<ifilename::MaxFilenameLength> fullname = (m_prefix+filename).str();
    if (mode == eReadOnly)
    {
        int file = ::open(fullname.c_str(), O_RDONLY);
        if (file == -1)
        {
            printf("%s\n", fullname.c_str());
            return ref<IMemoryStream>();
        }
        else
        {
            off_t size = lseek(file,0,SEEK_END);
            return ref<MemoryFileMap>::create(gameArena(), file, size);
        }
    }
    else
        return ref<IMemoryStream>();
}

size_t DiskFS::age(const ifilename& file) const
{
    minitl::format<ifilename::MaxFilenameLength> name = (m_prefix+file).str();

    return 0;
}

}
