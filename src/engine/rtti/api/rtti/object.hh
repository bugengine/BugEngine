/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_OBJECT_HH_
#define BE_RTTI_OBJECT_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>

namespace BugEngine
{

namespace RTTI
{
class MetaClass;
}

class be_api(RTTI) Object : public minitl::refcountable
{
public:
    virtual ~Object();

    typedef RTTI::MetaClass MetaClass;
    virtual ref<const RTTI::MetaClass>  metaclass() const = 0;
protected:
    Object();
    static ref<const Object::MetaClass> static_metaclass();
    static void                         registerMetaClass();
private:
    Object(const Object& other);
    Object& operator=(const Object& other);
};

}

/*****************************************************************************/
#endif
