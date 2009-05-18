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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_CORE_THREADS_MUTEX_HH_
#define BE_CORE_THREADS_MUTEX_HH_
/*****************************************************************************/
#include    <core/threads/waitable.hh>

namespace BugEngine
{

class COREEXPORT Mutex : public Threads::Waitable
{
public:
    Mutex();
    ~Mutex();

    void release();
};

class ScopedMutexLock
{
private:
    Mutex&  m_mutex;
public:
    inline ScopedMutexLock(Mutex& m) : m_mutex(m)   { m_mutex.wait(); }
    inline ~ScopedMutexLock()                       { m_mutex.release(); }
};

}

/*****************************************************************************/
#endif
