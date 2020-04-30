/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <ioprocess.hh>

namespace BugEngine {

static minitl::Allocator& ticketPool()
{
    return Arena::filesystem();  // TODO
}

File::Ticket::Ticket(minitl::Allocator& arena, weak< const File > file, i64 offset, u32 size, bool text)
    : action(Read)
    , file(file)
    , buffer(arena, 0)
    , processed(i_u32::create(0))
    , offset(offset)
    , total(size)
    , error(i_bool::create(false))
    , text(text)
{
    file->addref();
}

File::Ticket::Ticket(minitl::Allocator& arena, weak< const File > file, i64 offset, u32 size, bool text,
                     const void* data)
    : action(Write)
    , file(file)
    , buffer(arena, size)
    , processed(i_u32::create(0))
    , offset(offset)
    , total(size)
    , error(i_bool::create(false))
    , text(text)
{
    memcpy(buffer.data(), data, size);
    file->addref();
}

File::Ticket::~Ticket()
{
    const File* f = file.operator->();
    file          = weak< const File >();
    f->decref();
}

File::File(ifilename filename, Media media, u64 size, u64 fileState)
    : m_filename(filename)
    , m_media(media)
    , m_size(size)
    , m_state(fileState)
{
}

File::~File()
{
}

ref< const File::Ticket > File::beginRead(u32 size, i64 offset, bool text, minitl::Allocator& arena) const
{
    u32 s;
    if(offset >= 0)
    {
        be_assert((u64)offset <= m_size, "reading past end of file");
        be_assert((u64)offset + size <= m_size, "reading past end of file");
        s = size ? size : be_checked_numcast< u32 >(m_size - offset);
    }
    else
    {
        be_assert(offset + (i64)m_size + 1 >= 0, "reading past end of file");
        be_assert(m_size + offset + size + 1 <= m_size, "reading past end of file");
        s = size ? size : be_checked_numcast< u32 >((i64)m_size + offset + (text ? 1 : 0));
    }
    ref< Ticket > t = ref< Ticket >::create(ticketPool(), byref(arena), this, offset, s, text);
    IOProcess::IOContext::pushTicket(t);
    return t;
}

ref< const File::Ticket > File::beginWrite(const void* data, u32 size, i64 offset)
{
    if(offset > 0)
        be_assert((u64)offset <= m_size, "writing past end of file");
    else if(offset < 0)
        be_assert(offset + (i64)m_size + 1 >= 0, "writing past end of file");
    ref< Ticket > t = ref< Ticket >::create(ticketPool(), byref(Arena::temporary()), this, offset, size, false, data);
    IOProcess::IOContext::pushTicket(t);
    return t;
}

void File::fillBuffer(weak< Ticket > ticket) const
{
    be_assert(ticket->file == this, "trying to fill buffer of another file");
    doFillBuffer(ticket);
}

void File::writeBuffer(weak< Ticket > ticket) const
{
    be_assert(ticket->file == this, "trying to fill buffer of another file");
    doWriteBuffer(ticket);
}

u64 File::getState() const
{
    return m_state;
}

bool File::isDeleted() const
{
    return m_state == ~(u64)0;
}

void File::refresh(u64 fileSize, u64 fileState)
{
    if(fileSize != m_size || fileState != m_state)
    {
        m_size = fileSize;
        if(fileState != m_state)
        {
            m_state = fileState;
        }
        else
        {
            m_state++;
        }
    }
}

}  // namespace BugEngine
