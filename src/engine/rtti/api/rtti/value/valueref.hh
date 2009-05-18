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

#ifndef BE_RTTI_VALUEREF_HH_
#define BE_RTTI_VALUEREF_HH_
/*****************************************************************************/
#include    <minitl/type/variant>
#include    <rtti/value/valuetypes.hh>

namespace BugEngine
{

class Value;
    
namespace RTTI
{

class AbstractMarshaller;


class ValueRef
{
    friend class BugEngine::Value;
private:
    void*                       m_data;
    const AbstractMarshaller*   m_marshaller;
public:
    inline ValueRef(void* m_data, const AbstractMarshaller* m_marshaller);
    inline ValueRef(const ValueRef& other);
    inline ~ValueRef();

    template< typename T >
    inline T as() const;
    template< typename T >
    inline T member() const;
};

}}

#include    <rtti/value/valueref.inl>

/*****************************************************************************/
#endif
