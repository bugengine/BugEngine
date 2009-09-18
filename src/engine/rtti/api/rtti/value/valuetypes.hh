/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
