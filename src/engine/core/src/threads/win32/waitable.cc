/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/threads/waitable.hh>


namespace BugEngine { namespace Threads
{

Waitable::Waitable(void* data)
:   m_data(data)
{
}

Waitable::~Waitable()
{
    CloseHandle((HANDLE)m_data);
}

Waitable::WaitResult Waitable::wait(unsigned int timeout)
{
    DWORD rcode = WaitForSingleObject((HANDLE)m_data, timeout);
    switch(rcode)
    {
    case WAIT_OBJECT_0:
        return Finished;
    case WAIT_TIMEOUT:
        return TimeOut;
    case WAIT_FAILED:
        Assert(false);
    case WAIT_ABANDONED:
    default:
        return Abandoned;
    }
}

}}
