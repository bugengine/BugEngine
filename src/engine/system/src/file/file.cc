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

File::Ticket::Ticket(Allocator& arena, weak<const File> file, u64 offset, u32 size)
    :   file(file)
    ,   buffer(arena, size, 16)
    ,   processed(0)
    ,   offset(offset)
    ,   total(size)
{
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

ref<const File::Ticket> File::beginRead(u64 offset, u32 size, Allocator& arena) const
{
    u32 s = size ? size : be_checked_numcast<u32>(m_size - offset);
    be_assert(offset <= m_size, "reading past end of file");
    be_assert(offset+size <= m_size, "reading past end of file");
    ref<Ticket> t = ref<Ticket>::create(ticketPool(), byref(arena), this, offset, s);
    IOProcess::pushTicket(t);
    return t;
}

void File::fillBuffer(weak<Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to fill buffer of another file");
    doFillBuffer(ticket);
}

}
