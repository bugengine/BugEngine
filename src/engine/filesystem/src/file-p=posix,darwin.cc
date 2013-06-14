/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <posix/file.hh>
#include    <cerrno>
#include    <cstdio>

namespace BugEngine
{

PosixFile::PosixFile(ifilename file, Media media, u64 size, time_t modifiedTime)
    :   File(media, size, true)
    ,   m_file(file)
    ,   m_modifiedTime(modifiedTime)
{
}

PosixFile::~PosixFile()
{
}

void PosixFile::doFillBuffer(weak<File::Ticket> ticket) const
{
    static const int g_bufferSize = 1024;

    be_assert(ticket->file == this, "trying to read wrong file");
    ifilename::Filename pathname = m_file.str();
    FILE* f = fopen(pathname.name, "rb");
    if (!f)
    {
        const char *errorMessage = strerror(errno);
        be_forceuse(errorMessage);
        be_error("file %s could not be opened: (%d) %s" | m_file | errno | errorMessage);
        ticket->error = true;
    }
    else
    {
        if (ticket->offset > 0)
        {
            fseek(f, ticket->offset, SEEK_SET);
        }
        else if (ticket->offset < 0)
        {
            fseek(f, ticket->offset+1, SEEK_END);
        }
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
        fclose(f);
    }
}

void PosixFile::doWriteBuffer(weak<Ticket> ticket) const
{
    static const int g_bufferSize = 1024;

    be_assert(ticket->file == this, "trying to write to wrong file");
    ifilename::Filename pathname = m_file.str();
    FILE* f = fopen(pathname.name, "ab");
    if (!f)
    {
        const char *errorMessage = strerror(errno);
        be_forceuse(errorMessage);
        be_error("file %s could not be opened: (%d) %s" | m_file | errno | errorMessage);
        ticket->error = true;
    }
    else
    {
        if (ticket->offset > 0)
        {
            fseek(f, ticket->offset, SEEK_SET);
        }
        else if (ticket->offset < 0)
        {
            fseek(f, ticket->offset+1, SEEK_END);
        }
        u8* data = ticket->buffer.data();
        for (ticket->processed = 0; !ticket->done(); )
        {
            int written = (ticket->processed+g_bufferSize > ticket->total)
                ?   fwrite(data, 1, be_checked_numcast<u32>(ticket->total - ticket->processed), f)
                :   fwrite(data, 1, g_bufferSize, f);
            ticket->processed += written;
            data += written;
            if (written == 0)
            {
                const char *errorMessage = strerror(errno);
                be_forceuse(errorMessage);
                be_error("could not write part of the buffer to file %s; failed after processing %d bytes out of %d (%s)" | m_file | ticket->processed | ticket->total | errorMessage);
                ticket->error = true;
            }
        }
    }
    fflush(f);
    fclose(f);
}

void PosixFile::refresh(time_t modifiedTime)
{
    if (modifiedTime != m_modifiedTime)
    {
        m_modifiedTime = modifiedTime;
        File::notifyChanged();
    }
}

}
