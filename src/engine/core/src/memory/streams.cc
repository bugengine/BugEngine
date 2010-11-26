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

i64 IMemoryStream::read(void* buffer, i64 _size)
{
    i64 toread = minitl::min(_size,size()-offset());
    memcpy(buffer, memory(), be_checked_numcast<size_t>(toread));
    seek(eSeekMove, toread);
    return toread;
}

void IMemoryStream::write(void* buffer, i64 _size)
{
    be_assert(writable(), "writing in a read-only memory stream");
    if(_size > size()-offset())
        resize(offset()+_size);
    memcpy(memory(),buffer,be_checked_numcast<size_t>(_size));
    seek(eSeekMove, _size);
}

/*****************************************************************************/


}
