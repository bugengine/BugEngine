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
#include    <core/threads/mutex.hh>
#include    <pthread.h>


namespace BugEngine
{

Mutex::Mutex()
:   m_data(new pthread_mutex_t)
{
    pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(m_data), 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(m_data));
    delete reinterpret_cast<pthread_mutex_t*>(m_data);
}

void Mutex::release()
{
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(m_data));
}

Threads::Waitable::WaitResult Mutex::wait(unsigned int timeout)
{
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(m_data));
    return Finished;
}

}
