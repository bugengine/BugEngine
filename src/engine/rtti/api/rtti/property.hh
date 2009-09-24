/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PROPERTY_HH_
#define BE_RTTI_PROPERTY_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class be_api(RTTI) Property : public Object
{
protected:
    class be_api(RTTI) MetaClass : public RTTI::MetaClass
    {
    public:
        MetaClass();
        MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass);
        ~MetaClass();
    };
public:
    Property();
    virtual ~Property();

    virtual bool                readable(Object* from) const = 0;
    virtual bool                writable(Object* from) const = 0;
    virtual void                set(Object* dest, const Value& value) const = 0;
    virtual Value               get(Object* from) const = 0;

    virtual const MetaClass*    metaclass() const override;
    static  const MetaClass*    static_metaclass();
private:
    Property(const Property& other);
    Property& operator=(const Property& other);
};

}}

/*****************************************************************************/
#endif
