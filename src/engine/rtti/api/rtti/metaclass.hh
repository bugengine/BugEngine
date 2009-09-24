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
    typedef minitl::map< istring, refptr<const Property> >      PropertyMap;
    typedef PropertyMap::iterator                               PropertyIterator;
    typedef PropertyMap::const_iterator                         PropertyConstIterator;
private:
    istring                 m_name;
    refptr<const MetaClass> m_parent;
protected:
    refptr<MetaMetaClass>   m_metaclass;
    PropertyMap             m_properties;
public:
    MetaClass(const inamespace& name, const MetaClass* parent, MetaMetaClass* mc, bool registerClass = true);
    virtual ~MetaClass();

    const istring& name() const;
    virtual const MetaClass* metaclass() const override;
    virtual const MetaClass* parent() const;

    virtual Value call(Value* params, size_t nbParams) const;

    void addProperty(const istring& name, refptr<const Property> property);
    void addMethod(const istring& name, refptr<Method> method);
    const Property* getProperty(const istring& name) const;

    virtual refptr<Object> create() const;
protected:
    static void init(MetaClass* mc);
private:
    MetaClass(const MetaClass& other);
    MetaClass& operator=(const MetaClass& other);
};

class MetaMetaClass : public MetaClass
{
public:
    MetaMetaClass(const inamespace& name, const MetaClass* parent);
    ~MetaMetaClass();
private:
    MetaMetaClass(const MetaMetaClass& other);
    MetaMetaClass& operator=(const MetaMetaClass& other);
};

}}

/*****************************************************************************/
#endif
