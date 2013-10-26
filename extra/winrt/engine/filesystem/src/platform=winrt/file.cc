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

void Win32File::doFillBuffer(weak<File::Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    //ifilename::Filename pathname = m_file.str();
}

void Win32File::doWriteBuffer(weak<Ticket> ticket) const
{
    be_assert(ticket->file == this, "trying to read wrong file");
    //ifilename::Filename pathname = m_file.str();
}

}
