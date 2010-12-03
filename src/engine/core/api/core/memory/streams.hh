/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STREAMS_HH_
#define BE_CORE_STREAMS_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>

namespace BugEngine
{

class be_api(CORE) IMemoryStream : public minitl::refcountable
{
public:
    enum SeekMethod
    {
        eSeekFromStart = SEEK_SET,
        eSeekMove      = SEEK_CUR,
        eSeekFromEnd   = SEEK_END
    };
public:
    IMemoryStream();
    virtual ~IMemoryStream();

    virtual void*  basememory() = 0;
    virtual void*  memory();
    virtual i64    offset() const = 0;
    virtual i64    size() const = 0;
    virtual void   seek(SeekMethod method, i64 offset) = 0;
    virtual i64    read(void* buffer, i64 size);
    virtual void   write(void *buffer, i64 size);
    virtual void   resize(i64 size) = 0;
    virtual bool   writable() const = 0;
};

class be_api(CORE) MemoryStream : public IMemoryStream
{
private:
    Allocator::Block<u8>    m_memory;
    i64                     m_size;
    i64                     m_offset;
public:
    MemoryStream(Allocator& allocator, i64 size = 0);
    virtual ~MemoryStream();

    virtual void*  basememory() override;
    virtual i64    size() const override;
    virtual i64    offset() const override;
    virtual void   seek(SeekMethod method, i64 offset) override;
    virtual void   resize(i64 size) override;
    virtual bool   writable() const override;
};

}

/*****************************************************************************/
#endif
