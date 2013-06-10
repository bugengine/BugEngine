/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <win32/file.hh>


namespace BugEngine
{

Win32File::Win32File(ifilename file, Media media, u64 size)
    :   File(media, size, false)
    ,   m_file(file)
{
}

Win32File::~Win32File()
{
}

static void setFilePointer(const char *debugName, HANDLE file, i64 wantedOffset)
{
    LARGE_INTEGER setOffset;
    LARGE_INTEGER offset;
    if (wantedOffset >= 0)
    {
        offset.QuadPart = wantedOffset;
        SetFilePointerEx (file, offset, &setOffset, FILE_BEGIN);
        be_assert(setOffset.QuadPart == wantedOffset, "seek in file %s failed: position %d instead of %d" | debugName | setOffset.QuadPart | wantedOffset);
    }
    else
    {
        offset.QuadPart = wantedOffset+1;
        SetFilePointerEx (file, offset, &setOffset, FILE_END);
    }
}

void Win32File::doFillBuffer(weak<File::Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    ifilename::Filename pathname = m_file.str();
    HANDLE h = CreateFileA ( pathname.name,
                             GENERIC_READ,
                             FILE_SHARE_READ,
                             0,
                             OPEN_EXISTING,
                             0,
                             0);
    if (h == INVALID_HANDLE_VALUE)
    {
        char *errorMessage = 0;
        int errorCode = ::GetLastError();
        FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&errorMessage),
            0,
            NULL);
        be_info("file %s could not be opened: CreateFile returned an error (%d) %s" | m_file | errorCode | errorMessage);
        ticket->error = true;
        ::LocalFree(errorMessage);
    }
    else
    {
        static const int s_bufferSize = 1024;
        setFilePointer(pathname.name, h, ticket->offset);
        for (ticket->processed = 0; !ticket->done(); )
        {
            DWORD read;
            if (ticket->processed+s_bufferSize > ticket->total)
                ReadFile (h, ticket->buffer.data()+ticket->processed, be_checked_numcast<u32>(ticket->total - ticket->processed), &read, 0);
            else
                ReadFile (h, ticket->buffer.data()+ticket->processed, s_bufferSize, &read, 0);
            ticket->processed += read;
            if (read == 0)
            {
                be_error("reached premature end of file in %s after reading %d bytes (offset %d)" | m_file | ticket->processed | ticket->total);
                ticket->error = true;
            }
        }
    }
    CloseHandle(h);
}

void Win32File::doWriteBuffer(weak<Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    ifilename::Filename pathname = m_file.str();
    HANDLE h = CreateFileA ( pathname.name,
                             GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             0,
                             0);
    if (h == INVALID_HANDLE_VALUE)
    {
        char *errorMessage = 0;
        int errorCode = ::GetLastError();
        FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&errorMessage),
            0,
            NULL);
        be_info("file %s could not be opened: CreateFile returned an error (%d) %s" | m_file | errorCode | errorMessage);
        ticket->error = true;
        ::LocalFree(errorMessage);
    }
    else
    {
        static const int s_bufferSize = 1024;
        setFilePointer(pathname.name, h, ticket->offset);
        for (ticket->processed = 0; !ticket->done(); )
        {
            DWORD written;
            if (ticket->processed+s_bufferSize > ticket->total)
                WriteFile (h, ticket->buffer.data()+ticket->processed, be_checked_numcast<u32>(ticket->total - ticket->processed), &written, 0);
            else
                WriteFile (h, ticket->buffer.data()+ticket->processed, s_bufferSize, &written, 0);
            ticket->processed += written;
            if (written == 0)
            {
                be_error("could not write part of the buffer to file %s; failed after processing %d bytes out of %d" | m_file | ticket->processed | ticket->total);
                ticket->error = true;
            }
        }
        CloseHandle(h);
    }
}

}
