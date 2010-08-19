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

template< int ARENA >
class MemoryStream : public IMemoryStream
{
private:
    typename Memory<ARENA>::Block m_memory;
    i64                                 m_size;
    i64                                 m_offset;
public:
    inline MemoryStream();
    inline MemoryStream(i64 size);
    virtual inline ~MemoryStream();

    virtual void*  basememory() override;
    virtual i64    size() const override;
    virtual i64    offset() const override;
    virtual void   seek(SeekMethod method, i64 offset) override;
    virtual void   resize(i64 size) override;
    virtual bool   writable() const override;
};


template< int ARENA >
MemoryStream<ARENA>::MemoryStream()
:   m_memory(0)
,   m_size(0)
,   m_offset(0)
{
}

template< int ARENA >
MemoryStream<ARENA>::MemoryStream(i64 size)
:   m_memory(size)
,   m_size(size)
,   m_offset(0)
{
}

template< int ARENA >
MemoryStream<ARENA>::~MemoryStream()
{
}

template< int ARENA >
void* MemoryStream<ARENA>::basememory()
{
    return m_memory;
}

template< int ARENA >
i64 MemoryStream<ARENA>::size() const
{
    return m_size;
}

template< int ARENA >
i64 MemoryStream<ARENA>::offset() const
{
    return m_offset;
}

template< int ARENA >
void MemoryStream<ARENA>::seek(SeekMethod method, i64 offset)
{
    switch(method)
    {
        case eSeekMove:
            m_offset += offset;
            break;
        case eSeekFromStart:
            m_offset = offset;
            break;
        case eSeekFromEnd:
            m_offset = m_size + offset;
            break;
        default:
            be_notreached();
    }
    if(m_offset < 0) m_offset = 0;
    if(m_offset > m_size) m_offset = m_size;
}

template< int ARENA >
void MemoryStream<ARENA>::resize(i64 size)
{
    m_memory.resize(size);
}

template< int ARENA >
bool MemoryStream<ARENA>::writable() const
{
    return true;
}

}

/*****************************************************************************/
#endif
