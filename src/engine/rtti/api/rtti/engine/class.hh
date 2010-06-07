/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASS_HH_
#define BE_RTTI_ENGINE_CLASS_HH_
/*****************************************************************************/
#include   <rtti/classinfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename Class, typename Parent = void >
class ClassInfo : public BugEngine::ClassInfo
{
public:
    ClassInfo(const inamespace& name);
    virtual ~ClassInfo();

    template< typename T >
    PropertyInfo& property(const istring& name, T(Class::*member));

    size_t          size() const override   { return sizeof(Class); }
};

}}

#include   <rtti/engine/class.inl>
#include   <rtti/documentation/brief.hh>
#include   <rtti/documentation/detail.hh>


#define be_class(klass,parent,name)                                            \

#define be_field(name)                                                         \

#define be_field_name(var, name)                                               \

#define be_end


/*****************************************************************************/
#endif
