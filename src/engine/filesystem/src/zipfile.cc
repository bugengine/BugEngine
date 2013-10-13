/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <zipfile.hh>

namespace BugEngine
{

ZipFile::ZipFile(void* handle, const ifilename& filename, const unz_file_info& info, const unz_file_pos& filePos)
    :   File(File::Media(File::Media::Package, 0, 0), u64(info.uncompressed_size), u64(info.dosDate))
    ,   m_file(filename)
    ,   m_handle(handle)
    ,   m_filePos(filePos)
{
    be_info("created zip file %s" | m_file);
}

ZipFile::~ZipFile()
{
}

void ZipFile::doFillBuffer(weak<File::Ticket> ticket) const
{
    /* state of the previous read */
    static const unz_file_pos* s_currentFile = 0;
    static u8 s_buffer[1024];
    static u32 s_leftBytes = 0;
    static u32 s_leftBytesOffset = 0;
    static u32 s_fileOffset = 0;


    be_assert(ticket->file == this, "trying to read wrong file");
    if (s_currentFile != &m_filePos)
    {
        s_currentFile = &m_filePos;
        s_fileOffset = 0;
        s_leftBytes = 0;
        int result = unzGoToFilePos(m_handle, &m_filePos);
        be_assert(result == UNZ_OK, "could not go to file %s" | m_file);
    }
    u8* destination = ticket->buffer.begin();
    if (s_leftBytes)
    {
        memcpy(destination, s_buffer + s_leftBytesOffset, s_leftBytes);
        destination += s_leftBytes;
        ticket->processed += s_leftBytes;
    }
    be_info("file reading %s not implemented yet" | m_file);
}

void ZipFile::doWriteBuffer(weak<Ticket> ticket) const
{
    be_forceuse(ticket);
    be_notreached();
}

}
