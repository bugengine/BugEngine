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

#ifndef BE_CORE_THREADS_THREAD_HH_
#define BE_CORE_THREADS_THREAD_HH_
/*****************************************************************************/

namespace BugEngine
{

class COREEXPORT Thread
{
private:
    class ThreadParams;
private:
    ThreadParams*   m_params;
    void*           m_data;
    unsigned long   m_id;
public:
    typedef intptr_t(*ThreadFunction)(intptr_t, intptr_t);
public:
    enum Priority
    {
        Idle,
        Lowest,
        BelowNormal,
        Normal,
        AboveNormal,
        Highest,
        Critical
    };
public:
    Thread(const istring& name, ThreadFunction f, intptr_t p1 = 0, intptr_t p2 = 0, Priority p = Normal, bool isSuspended = false);
    ~Thread();

    void resume();

    Priority priority() const;
    void setPriority(Priority p);

    unsigned long   id() const;
    void wait() const;

    static void             sleep(int milliseconds);
    static void             yield();
    static unsigned long    currentId();
    static const istring&   name();
};

}

/*****************************************************************************/
#endif
