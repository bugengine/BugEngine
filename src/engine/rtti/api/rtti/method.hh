/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHOD_HH_
#define BE_RTTI_METHOD_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class RTTIEXPORT Method : public Object
{
protected:
    class RTTIEXPORT MetaClass : public RTTI::MetaClass
    {
    public:
        MetaClass();
        MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass);
        ~MetaClass();
    };
public:
    Method();
    virtual ~Method();

    virtual const MetaClass* metaclass() const override;
    static  const MetaClass* static_metaclass();
private:
    Method(const Method& other);
    Method& operator=(const Method& other);
};

}}

/*****************************************************************************/
#endif
