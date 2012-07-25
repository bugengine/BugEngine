/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/streams.hh>

namespace BugEngine
{

MemoryStream::MemoryStream(minitl::Allocator& allocator, u64 size)
    :   m_memory(allocator, size)
    ,   m_size(size)
    ,   m_capacity(0)
{
}


MemoryStream::~MemoryStream()
{
}

void MemoryStream::resize(u64 size)
{
    if (!m_memory.resize(size))
    {
        m_memory.realloc(size);
    }
}

void MemoryStream::write(const void* buffer, u64 size_)
{
    if (size_ + m_size > m_capacity)
        resize(m_size+size_);
    memcpy(m_memory+m_size, buffer, be_checked_numcast<size_t>(size_));
    m_size = m_size + size_;
}

void MemoryStream::erase(u64 count)
{
    be_assert(m_size >= count, "erasing %d bytes from a stream of %d bytes" | count | m_size);
    m_size -= count;
}

}
