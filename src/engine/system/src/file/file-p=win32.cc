/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <win32/file.hh>


namespace BugEngine
{

Win32File::Win32File(ifilename file, Media media, u64 size)
    :   File(media, size)
    ,   m_file(file)
{
}

Win32File::~Win32File()
{
}

void Win32File::doFillBuffer(weak<File::Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    minitl::format<4096> pathname = m_file.str();
    HANDLE h = CreateFileA ( pathname.c_str(),
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
        be_info("File %s could not be opened: (%d) %s" | m_file | errorCode | errorMessage);
        ticket->error = true;
    }
    else
    {
        LARGE_INTEGER setOffset;
        LARGE_INTEGER offset;
        offset.QuadPart = ticket->offset;
        SetFilePointerEx (h, offset, &setOffset, FILE_BEGIN);
        be_assert(offset.QuadPart == ticket->offset, "Seek in file %s failed: position %d instead of %d" | m_file | offset.QuadPart | ticket->offset);
        for (ticket->processed = 0; !ticket->done(); )
        {
            DWORD read;
            if (ticket->processed+1024 > ticket->total)
                ReadFile (h, ticket->buffer.data(), be_checked_numcast<u32>(ticket->total - ticket->processed), &read, 0);
            else
                ReadFile (h, ticket->buffer.data(), 1024, &read, 0);
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

}
