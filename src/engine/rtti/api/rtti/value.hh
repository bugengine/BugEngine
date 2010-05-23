/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine
{

class TypeInfo;
class PropertyInfo;

class Value
{
private:
    char            m_buffer[128];
    void*           m_data;
    const TypeInfo* m_type;
    struct PropertyProxy
    {
        Value&                      from;
        weak<const PropertyInfo>    prop;

        inline PropertyProxy(Value& v, weak<const PropertyInfo> p);
        inline operator Value();
        inline void operator=(const Value& v);
    private:
        PropertyProxy& operator=(const PropertyProxy& other);
    };
public:
    template< typename T >
    explicit inline Value(const T& t);
    inline ~Value();

    template< typename T >
    inline const T& as() const;

    inline PropertyProxy operator[](istring);
};

}

/*****************************************************************************/
#endif
