/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/streams.hh>
#include    <minitl/container/algorithm.hh>

namespace BugEngine
{

IMemoryStream::IMemoryStream()
{
}

IMemoryStream::~IMemoryStream()
{
}

void* IMemoryStream::memory()
{
    return static_cast<void*>(static_cast<char*>(basememory())+offset());
}

const void* IMemoryStream::memory() const
{
    return static_cast<const void*>(static_cast<const char*>(basememory())+offset());
}

i64 IMemoryStream::read(void* buffer, i64 _size)
{
    i64 toread = minitl::min(_size,size()-offset());
    memcpy(buffer, memory(), be_checked_numcast<size_t>(toread));
    seek(eSeekMove, toread);
    return toread;
}

void IMemoryStream::write(const void* buffer, i64 _size)
{
    be_assert(writable(), "writing in a read-only memory stream");
    if (_size > size()-offset())
        resize(offset()+_size);
    memcpy(memory(),buffer,be_checked_numcast<size_t>(_size));
    seek(eSeekMove, _size);
}

/*****************************************************************************/

MemoryStream::MemoryStream(Allocator& allocator, i64 size)
    :   m_memory(allocator, (size_t)size)
    ,   m_size(size)
    ,   m_offset(0)
{
}


MemoryStream::~MemoryStream()
{
}

void* MemoryStream::basememory()
{
    return m_memory;
}

const void* MemoryStream::basememory() const
{
    return m_memory;
}

i64 MemoryStream::size() const
{
    return m_size;
}

i64 MemoryStream::offset() const
{
    return m_offset;
}

void MemoryStream::seek(SeekMethod method, i64 offset)
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
    if (m_offset < 0) m_offset = 0;
    if (m_offset > m_size) m_offset = m_size;
}

void MemoryStream::resize(i64 size)
{
    if (!m_memory.resize((size_t)size))
    {
        m_memory.realloc((size_t)size);
    }
}

bool MemoryStream::writable() const
{
    return true;
}


}
