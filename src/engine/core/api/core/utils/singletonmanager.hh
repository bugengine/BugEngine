/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_SINGLETON_MANAGER_HH_
#define BE_CORE_SINGLETON_MANAGER_HH_
/*****************************************************************************/
#include <minitl/container/hash_map.hh>

namespace BugEngine
{

class SingletonBase;

class be_api(CORE) SingletonManager
{
private:
    minitl::map< const char*, SingletonBase*, minitl::less<const char *> > m_registry;
public:
    SingletonManager();
    ~SingletonManager();

    SingletonBase* get(const char* name) const NOTHROW;
    void set(const char* name, SingletonBase* instance);
    void erase(const char* name, const SingletonBase* instance);

    static SingletonManager* instance() NOTHROW;
};

}

/*****************************************************************************/
#endif

