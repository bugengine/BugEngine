/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CONSTPROPERTY_HH_
#define BE_RTTI_ENGINE_CONSTPROPERTY_HH_
/*****************************************************************************/
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
class ConstPropertyInfo : public BugEngine::PropertyInfo
{
private:
    Value m_value;
public:
    ConstPropertyInfo(T value);
    ~ConstPropertyInfo();

    const TypeInfo* type() const override;
    Value get(void* from) const override;
    void set(const Value& v, void* to) const override;
};

}}

#include <rtti/engine/constproperty.inl>

/*****************************************************************************/
#endif
