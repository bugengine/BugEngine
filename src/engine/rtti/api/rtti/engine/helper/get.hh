/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_GET_HH_
#define BE_RTTI_ENGINE_HELPER_GET_HH_
/*****************************************************************************/
#include   <rtti/value.inl>

namespace BugEngine { namespace RTTI
{


template< typename T, ptrdiff_t offset >
struct PropertyHelper
{
    static Value get(void* from, const Type& type)
    {
        void* t = reinterpret_cast<void*>(reinterpret_cast<char*>(from)+offset);
        return Value(type, t);
    }
};

template< ptrdiff_t offset >
struct PropertyHelper<Value, offset>
{
    static Value get(void* from, const Type& type)
    {
        if (type.constness == Type::Const)
        {
            return Value::ByRef(*reinterpret_cast<const Value*>(reinterpret_cast<char*>(from)+offset));
        }
        else
        {
            return Value::ByRef(*reinterpret_cast<Value*>(reinterpret_cast<char*>(from)+offset));
        }
    }
};

}}

/*****************************************************************************/
#endif
