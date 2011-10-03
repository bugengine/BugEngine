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

}
