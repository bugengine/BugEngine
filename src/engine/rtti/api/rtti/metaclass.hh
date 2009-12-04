/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METACLASS_HH_
#define BE_RTTI_METACLASS_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class MetaMetaClass;
class Property;
class Method;

class be_api(RTTI) MetaClass : public Object
{
    friend class Object;
private:
    class StaticProperty;
public:
    typedef RTTI::MetaMetaClass MetaMetaClass;
    typedef minitl::map< istring, ref<const Property> > PropertyMap;
    typedef PropertyMap::iterator                       PropertyIterator;
    typedef PropertyMap::const_iterator                 PropertyConstIterator;
private:
    istring                 m_name;
    ref<const MetaClass>    m_parent;
    weak<const Property>    m_dbOwner;
protected:
    ref<MetaMetaClass>      m_metaclass;
    PropertyMap             m_properties;
public:
    MetaClass(const inamespace& name, ref<const MetaClass> parent, ref<MetaMetaClass> mc, bool registerClass = true);
    virtual ~MetaClass();

    const istring& name() const;
    virtual ref<const MetaClass> metaclass() const override;
    virtual ref<const MetaClass> parent() const;

    virtual Value call(Value* params, size_t nbParams) const;

    void addProperty(const istring& name, ref<const Property> property);
    void addMethod(const istring& name, ref<Method> method);
    weak<const Property> getProperty(const istring& name) const;

    virtual ref<Object> create() const;
protected:
    static void init(weak<MetaClass> mc);
private:
    MetaClass(const MetaClass& other);
    MetaClass& operator=(const MetaClass& other);
};

class be_api(RTTI) MetaMetaClass : public MetaClass
{
public:
    MetaMetaClass(const inamespace& name, ref<const MetaClass> parent);
    ~MetaMetaClass();
private:
    MetaMetaClass(const MetaMetaClass& other);
    MetaMetaClass& operator=(const MetaMetaClass& other);
};

}}

/*****************************************************************************/
#endif
