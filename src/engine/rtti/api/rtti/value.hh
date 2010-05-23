/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

class Value
{
private:
    char            m_buffer[128];
    void*           m_data;
    TypeInfo        m_type;
public:
    template< typename T >
    explicit inline Value(const T& t);
    inline ~Value();

    template< typename T >
    inline const T& as() const;
};

}

#include   <rtti/value.inl>

/*****************************************************************************/
#endif
