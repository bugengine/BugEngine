/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_FIELDPROPERTY_HH_
#define BE_RTTI_ENGINE_FIELDPROPERTY_HH_
/*****************************************************************************/
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename Class, typename T >
class FieldPropertyInfo : public BugEngine::PropertyInfo
{
private:
    T(Class::*m_member);
public:
    FieldPropertyInfo(T(Class::*member));
    ~FieldPropertyInfo();

    const TypeInfo* type() const override;
    Value get(void* from) const override;
    void set(const Value& v, void* to) const override;
};

}}

#include <rtti/engine/fieldproperty.inl>

/*****************************************************************************/
#endif
