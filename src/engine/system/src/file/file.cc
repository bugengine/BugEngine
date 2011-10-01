/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/file/file.script.hh>


namespace BugEngine
{

static Allocator& ticketPool()
{
    return gameArena(); //TODO
}

File::Ticket::Ticket(Allocator& arena, weak<const File> file, size_t offset, size_t size)
    :   m_file(file)
    ,   buffer(arena, size, 16)
    ,   processed(0)
    ,   offset(offset)
    ,   total(size)
{
}

File::File(Media media, size_t size)
    :   m_media(media)
    ,   m_size(size)
{
}

File::~File()
{
}

ref<const File::Ticket> File::beginRead(size_t offset, size_t size, Allocator& arena) const
{
    size_t s = size ? size : m_size - offset;
    be_assert(offset < m_size, "reading past end of file");
    ref<Ticket> t = ref<Ticket>::create(ticketPool(), arena, this, offset, s);
    Folder::pushTicket(t);
    return t;
}

}
