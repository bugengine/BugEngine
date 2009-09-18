/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_SINGLETON_HH_
#define BE_CORE_SINGLETON_HH_
/*****************************************************************************/
#include    <core/utils/singletonmanager.hh>
#include    <typeinfo>

namespace BugEngine
{

class COREEXPORT SingletonBase
{
public:
    SingletonBase();
    virtual ~SingletonBase();
private:
    SingletonBase(const SingletonBase&);
    SingletonBase& operator=(const SingletonBase&);
};
    
template <typename T>
class Singleton : protected SingletonBase
{
private:
    static SingletonBase*    gInstance;
protected:
    Singleton (void)  { SingletonManager::instance()->set(typeid(T).name(),this); }
    virtual ~Singleton (void) { SingletonManager::instance()->erase(typeid(T).name(),this); }
public:
    static T *instance(void) NOTHROW
    {
        if(! gInstance)
            gInstance = SingletonManager::instance()->get(typeid(T).name());
        be_assert(gInstance, "could not find instance for type %s" | typeid(T).name());
        return static_cast<T*>(gInstance);
    }
    static bool hasinstance(void)
    {
        return instance() != 0;
    }
    static void destroy()
    {
        delete instance();
    }
};

template< typename T >
SingletonBase* Singleton<T>::gInstance = 0;

}

/*****************************************************************************/
#endif
