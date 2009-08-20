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

#ifndef BE_MINITL_MATHS_MINMAX_
#define BE_MINITL_MATHS_MINMAX_
/*****************************************************************************/

namespace minitl
{

template< size_t v1, size_t v2, bool v1mtv2 = (v1>v2) >
struct compare;

template< size_t v1, size_t v2 >
struct compare< v1, v2, true >
{
    enum
    {
        Max = v1,
        Min = v2
    };
};

template< size_t v1, size_t v2 >
struct compare< v1, v2, false >
{
    enum
    {
        Max = v2,
        Min = v1
    };
};

template< size_t v1, size_t v2 >
struct max
{
    enum { Value = compare<v1, v2>::Max };
};

template< size_t v1, size_t v2 >
struct min
{
    enum { Value = compare<v1, v2>::Min };
};

}


/*****************************************************************************/
#endif
