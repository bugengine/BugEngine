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

#ifndef BE_CORE_THREAD_SCHEDULER_RANGE_SEQUENCE_HH_
#define BE_CORE_THREAD_SCHEDULER_RANGE_SEQUENCE_HH_
/*****************************************************************************/

namespace BugEngine
{

template< typename T >
class range_sequence
{
private:
    T       m_begin;
    T       m_end;
    size_t  m_grain;
public:
    range_sequence(T begin, T end, size_t grain = 1);
    ~range_sequence();

    inline T&                   begin();
    inline T&                   end();
    inline size_t               size() const;
    inline range_sequence<T>    split();
    inline bool                 atomic() const;
};

}

#include    "sequence.inl"

/*****************************************************************************/
#endif
