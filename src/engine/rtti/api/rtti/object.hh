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

class be_api(RTTI) Object : public minitl::refcountable<void>
{
public:
    typedef RTTI::MetaClass MetaClass;
    Object();
    virtual ~Object();

    virtual const RTTI::MetaClass*  metaclass() const = 0;
    static const RTTI::MetaClass*   static_metaclass();
    static void                     registerMetaClass();
private:
    Object(const Object& other);
    Object& operator=(const Object& other);
};

}

/*****************************************************************************/
#endif
