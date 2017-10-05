/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <zipfile.hh>

namespace BugEngine
{

ZipFile::ZipFile(void* handle, const ifilename& filename, const unz_file_info& info, const unz_file_pos& filePos)
    :   File(filename, File::Media(File::Media::Package, 0, 0), u64(info.uncompressed_size), u64(info.dosDate))
    ,   m_handle(handle)
    ,   m_filePos(filePos)
{
    be_info("created zip file %s" | m_filename);
}

ZipFile::~ZipFile()
{
}

void ZipFile::doFillBuffer(weak<File::Ticket> ticket) const
{
    /* state of the previous read */
    static const unz_file_pos* s_currentFile = 0;
    static i64 s_fileOffset = 0;
    unz_file_pos filePos = m_filePos;


    be_assert(ticket->file == this, "trying to read wrong file");
    if (s_currentFile != &m_filePos || s_fileOffset > ticket->offset)
    {
        s_currentFile = &m_filePos;
        s_fileOffset = 0;
        unzCloseCurrentFile(m_handle);
        int result = unzGoToFilePos(m_handle, &filePos);
        be_assert(result == UNZ_OK, "could not go to file %s" | m_filename);
        result = unzOpenCurrentFile(m_handle);
        be_assert(result == UNZ_OK, "could not open file %s" | m_filename);
        be_forceuse(result);
    }

    while (s_fileOffset < ticket->offset)
    {
        u8 buffer[4096];
        i64 bytesToRead = minitl::min<i64>(4096, ticket->offset-s_fileOffset);
        i64 read = unzReadCurrentFile(m_handle, buffer, be_checked_numcast<unsigned int>(bytesToRead));
        s_fileOffset += read;
    }

    be_assert(ticket->buffer.byteCount() > ticket->total, "buffer is not long enough to read entire file; "
            "buffer size is %d, requires %d bytes" | ticket->buffer.byteCount() | ticket->total);
    u8* buffer = ticket->buffer.begin();
    while (!ticket->done())
    {
        i64 bytesToRead = ticket->total - ticket->processed;
        u32 bytesRead = unzReadCurrentFile(m_handle, buffer, be_checked_numcast<u32>(bytesToRead));
        if (bytesRead > 0)
        {
            ticket->processed += bytesRead;
            buffer += bytesRead;
        }
        else
        {
            be_error("error %d while reading from file %s" | bytesRead | m_filename);
        }
    }

    if (unzeof(m_handle))
    {
        s_fileOffset = 0;
        s_currentFile = 0;
    }
}

void ZipFile::doWriteBuffer(weak<Ticket> ticket) const
{
    be_forceuse(ticket);
    be_notreached();
}

}
