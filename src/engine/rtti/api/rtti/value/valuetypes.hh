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

#ifndef BE_RTTI_VALUETYPES_HH_
#define BE_RTTI_VALUETYPES_HH_
/*****************************************************************************/
#include    <rtti/object.hh>

namespace BugEngine
{

class Value;

namespace RTTI
{

typedef std::vector< Value >            PublishedVector;
typedef std::multimap< Value, Value >   PublishedMap;

typedef TYPELIST7( bool,
                   i64,
                   u64,
                   double,
                   std::string,
                   refptr<Object>,
                   Object* )   ValueTypes;
enum ValueTypeIndex
{
    PropertyTypeNotSet      = minitl::indexof< void, ValueTypes >::Value,
    PropertyTypeBool        = minitl::indexof< bool, ValueTypes >::Value,
    PropertyTypeInteger     = minitl::indexof< i64, ValueTypes >::Value,
    PropertyTypeUnsigned    = minitl::indexof< u64, ValueTypes >::Value,
    PropertyTypeFloat       = minitl::indexof< double, ValueTypes >::Value,
    PropertyTypeString      = minitl::indexof< std::string, ValueTypes >::Value,
    PropertyTypeObject      = minitl::indexof< refptr<Object>, ValueTypes >::Value,
    PropertyTypeWeakObject  = minitl::indexof< Object*, ValueTypes >::Value,
    //PropertyTypeVector        = Meta::IndexOf< PublishedVector, ValueTypes >::Value,
    //PropertyTypeMap           = Meta::IndexOf< PublishedMap, ValueTypes >::Value,
    PropertyTypeVariant     = 255
};


}}

/*****************************************************************************/
#endif
