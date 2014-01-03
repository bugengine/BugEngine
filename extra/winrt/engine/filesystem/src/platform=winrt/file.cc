/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <filesystem/stdafx.h>
#include    <win32/file.hh>


namespace BugEngine
{

Win32File::Win32File(ifilename filename, Media media, u64 size)
    :   File(filename, media, size, false)
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
