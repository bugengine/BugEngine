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

#ifndef BE_RTTI_MARSHALLER_HH_
#define BE_RTTI_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/value/valuecopy.hh>

namespace BugEngine { namespace RTTI
{

class AbstractMarshaller
{
public:
    virtual ValueCopy get(void* source)                                                 const = 0;
    virtual ValueCopy get(void* /*source*/, size_t /*index*/)                           const { throw 0; }
    virtual ValueCopy get(void* /*source*/, const Value& /*key*/)                       const { throw 0; }
    virtual void      set(const Value& value, void* dst)                                const = 0;
    virtual void      set(const Value& /*value*/, void* /*dst*/, size_t /*index*/)      const { throw 0; }
    virtual void      set(const Value& /*value*/, void* /*dst*/, const Value& /*key*/)  const { throw 0; }
    virtual void      push_back(const Value& /*value*/, void* /*dst*/)                  const { throw 0; }
    virtual void      push_front(const Value& /*value*/, void* /*dst*/)                 const { throw 0; }
    virtual void      pop_back(const Value& /*value*/, void* /*dst*/)                   const { throw 0; }
    virtual void      pop_front(const Value& /*value*/, void* /*dst*/)                  const { throw 0; }

    virtual ValueTypeIndex  type() const = 0;
};

template< typename T >
class Marshaller : public AbstractMarshaller
{
};

template< typename T >
class Marshaller<const T&> : public Marshaller<T>
{
};

}}

/*****************************************************************************/
#endif
