/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/filesystem/stdafx.h>
#include <windows/file.hh>

#include <cstdio>

namespace BugEngine {

Win32File::Win32File(ifilename filename, Media media, u64 size, u64 timestamp) : File(filename, media, size, timestamp)
{
}

Win32File::~Win32File()
{
}

static void setFilePointer(const char* debugName, HANDLE file, i64 wantedOffset)
{
    LARGE_INTEGER setOffset;
    LARGE_INTEGER offset;
    if(wantedOffset >= 0)
    {
        offset.QuadPart = wantedOffset;
        SetFilePointerEx(file, offset, &setOffset, FILE_BEGIN);
        be_assert(setOffset.QuadPart == wantedOffset,
                  "seek in file %s failed: position %d instead of %d" | debugName | setOffset.QuadPart | wantedOffset);
    }
    else
    {
        offset.QuadPart = wantedOffset + 1;
        SetFilePointerEx(file, offset, &setOffset, FILE_END);
    }
}

void Win32File::doFillBuffer(weak< File::Ticket > ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    ifilename::Filename pathname = m_filename.str('\\');
    HANDLE              h        = CreateFileA(pathname.name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(h == INVALID_HANDLE_VALUE)
    {
        int errorCode = ::GetLastError();
        be_info("file %s (%s) could not be opened: CreateFile returned an error (%d)" | m_filename | pathname.name
                | errorCode);
        ticket->error = true;
    }
    else
    {
        static const int s_bufferSize = 1024;
        static u8        buffer[s_bufferSize];
        u8*              target    = ticket->buffer.data();
        u32              processed = 0;
        setFilePointer(pathname.name, h, ticket->offset);
        for(ticket->processed = 0; !ticket->done();)
        {
            DWORD read;
            if(ticket->text)
            {
                if(ticket->processed + s_bufferSize > ticket->total)
                {
                    ReadFile(h, buffer, be_checked_numcast< u32 >(ticket->total - ticket->processed), &read, 0);
                }
                else
                {
                    ReadFile(h, buffer, s_bufferSize, &read, 0);
                }
                for(u32 i = 0; i < read; ++i)
                {
                    if(buffer[i] != '\r')
                    {
                        target[processed++] = buffer[i];
                    }
                }
            }
            else
            {
                if(ticket->processed + s_bufferSize > ticket->total)
                {
                    ReadFile(h, target + ticket->processed,
                             be_checked_numcast< u32 >(ticket->total - ticket->processed), &read, 0);
                }
                else
                {
                    ReadFile(h, target + ticket->processed, s_bufferSize, &read, 0);
                }
            }
            ticket->processed += read;
            if(read == 0)
            {
                be_error("reached premature end of file in %s after reading %d bytes (offset %d)" | m_filename
                         | ticket->processed | ticket->total);
                ticket->error = true;
            }
        }
        if(ticket->text && !ticket->error)
        {
            be_assert(processed + 1 <= ticket->buffer.count(),
                      "buffer size is %s; bytes processed is %s" | ticket->buffer.count() | (processed + 1));
            // shrink buffer
            ticket->buffer.realloc(processed + 1);
            target[processed] = 0;
        }
    }
    CloseHandle(h);
}

void Win32File::doWriteBuffer(weak< Ticket > ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    ifilename::Filename pathname = m_filename.str('\\');
    HANDLE              h        = CreateFileA(pathname.name, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if(h == INVALID_HANDLE_VALUE)
    {
        int errorCode = ::GetLastError();
        be_info("file %s (%s) could not be opened: CreateFile returned an error (%d)" | m_filename | pathname.name
                | errorCode);
        ticket->error = true;
    }
    else
    {
        static const int s_bufferSize = 1024;
        setFilePointer(pathname.name, h, ticket->offset);
        for(ticket->processed = 0; !ticket->done();)
        {
            DWORD written;
            if(ticket->processed + s_bufferSize > ticket->total)
                WriteFile(h, ticket->buffer.data() + ticket->processed,
                          be_checked_numcast< u32 >(ticket->total - ticket->processed), &written, 0);
            else
                WriteFile(h, ticket->buffer.data() + ticket->processed, s_bufferSize, &written, 0);
            ticket->processed += written;
            if(written == 0)
            {
                be_error("could not write part of the buffer to file %s; failed after processing %d bytes out of %d"
                         | m_filename | ticket->processed | ticket->total);
                ticket->error = true;
            }
        }
        CloseHandle(h);
    }
}

void Win32File::refresh(u64 size, u64 timestamp)
{
    File::refresh(size, timestamp);
}

}  // namespace BugEngine
