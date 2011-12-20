/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/file.script.hh>
#include    <ioprocess.hh>


namespace BugEngine
{

static Allocator& ticketPool()
{
    return gameArena(); //TODO
}

File::Ticket::Ticket(Allocator& arena, weak<const File> file, i64 offset, u32 size)
    :   action(Read)
    ,   file(file)
    ,   buffer(arena, 0)
    ,   processed(0)
    ,   offset(offset)
    ,   total(size)
{
    file->addref();
}

File::Ticket::Ticket(Allocator& arena, weak<const File> file, i64 offset, u32 size, const void* data)
    :   action(Write)
    ,   file(file)
    ,   buffer(arena, size)
    ,   processed(0)
    ,   offset(offset)
    ,   total(size)
{
    memcpy(buffer.data(), data, size);
    file->addref();
}

File::Ticket::~Ticket()
{
    const File* f = file.operator->();
    file = weak<const File>();
    f->decref();
}

File::File(Media media, u64 size)
    :   m_media(media)
    ,   m_size(size)
{
}

File::~File()
{
}

ref<const File::Ticket> File::beginRead(u32 size, i64 offset, Allocator& arena) const
{
    u32 s;
    if (offset > 0)
    {
        be_assert(offset <= m_size, "reading past end of file");
        be_assert(offset+size <= m_size, "reading past end of file");
        s = size ? size : be_checked_numcast<u32>(m_size - offset);
    }
    else
    {
        be_assert(offset+(i64)m_size+1 >= 0, "reading past end of file");
        be_assert(m_size+offset+size+1 <= m_size, "reading past end of file");
        s = size ? size : be_checked_numcast<u32>((i64)m_size + offset + 1);
    }
    ref<Ticket> t = ref<Ticket>::create(ticketPool(), byref(arena), this, offset, s);
    IOProcess::pushTicket(t);
    return t;
}

ref<const File::Ticket> File::beginWrite(const void* data, u32 size, i64 offset)
{
    if (offset > 0)
        be_assert(offset <= m_size, "writing past end of file");
    else if (offset < 0)
        be_assert(offset+(i64)m_size+1 >= 0, "writing past end of file");
    ref<Ticket> t = ref<Ticket>::create(ticketPool(), byref(tempArena()), this, offset, size, data);
    IOProcess::pushTicket(t);
    return t;
}

void File::fillBuffer(weak<Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to fill buffer of another file");
    doFillBuffer(ticket);
}

void File::writeBuffer(weak<Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to fill buffer of another file");
    doWriteBuffer(ticket);
}

}
