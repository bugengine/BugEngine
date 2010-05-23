/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTYINFO_HH_
#define BE_RTTI_PROPERTYINFO_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine
{

class PropertyInfo : public minitl::refcountable
{
private:
    minitl::vector<Value>   m_attributes;
public:
    PropertyInfo();
    virtual ~PropertyInfo();

    virtual TypeInfo type() const = 0;
    virtual Value get(void* from) const = 0;
    virtual void set(const Value& v, void* to) const = 0;

    template< typename T >
    PropertyInfo& operator[](const T& attribute)
    {
        m_attributes.push_back(Value(attribute));
        return *this;
    }
};

}

/*****************************************************************************/
#endif
