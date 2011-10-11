/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <posix/file.hh>


namespace BugEngine
{

PosixFile::PosixFile(ifilename file, Media media, u64 size)
    :   File(media, size)
    ,   m_file(file)
{
}

PosixFile::~PosixFile()
{
}

void PosixFile::doFillBuffer(weak<File::Ticket> ticket) const
{
    static const int g_bufferSize = 1024;

    be_assert(ticket->file == this, "trying to read wrong file");
    minitl::format<1024> pathname = m_file.str();
    FILE* f = fopen(pathname.c_str(), "rb");
    if (!f)
    {
        const char *errorMessage = strerror(errno);
        be_info("File %s could not be opened: (%d) %s" | m_file | errno | errorMessage);
        ticket->error = true;
    }
    else
    {
        if (ticket->offset)
            fseek(f, ticket->offset, SEEK_SET);
        u8* data = ticket->buffer.data();
        for (ticket->processed = 0; !ticket->done(); )
        {
            int read = (ticket->processed+g_bufferSize > ticket->total)
                ?   fread(data, 1, be_checked_numcast<u32>(ticket->total - ticket->processed), f)
                :   fread(data, 1, g_bufferSize, f);
            ticket->processed += read;
            data += read;
            if (read == 0)
            {
                be_error("reached premature end of file in %s after reading %d bytes (offset %d)" | m_file | ticket->processed | ticket->total);
                ticket->error = true;
            }
        }
    }
    fclose(f);
}

}
