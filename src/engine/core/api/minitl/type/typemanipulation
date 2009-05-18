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

#ifndef BE_MINITL_TYPE_TYPEOPERATIONS_
#define BE_MINITL_TYPE_TYPEOPERATIONS_
/*****************************************************************************/


namespace minitl
{

template< typename T > struct is_const              { enum { Value = 0 }; };
template< typename T > struct is_const<const T>     { enum { Value = 1 }; };
template< typename T > struct remove_const          { typedef T type; };
template< typename T > struct remove_const<const T> { typedef T type; };

template< typename T > struct is_reference          { enum { Value = 0 }; };
template< typename T > struct is_reference<T&>      { enum { Value = 1 }; };
template< typename T > struct remove_reference      { typedef T type; };
template< typename T > struct remove_reference<T&>  { typedef T type; };

template< typename T1, typename T2 >
struct type_equals
{
    enum { Value = 0 };
};

template< typename T >
struct type_equals<T, T>
{
    enum { Value = 1 };
};

}

/*****************************************************************************/
#endif
