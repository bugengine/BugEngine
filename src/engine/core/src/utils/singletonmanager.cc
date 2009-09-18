/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/utils/singletonmanager.hh>


namespace BugEngine
{

static SingletonManager gInstance;

SingletonManager::SingletonManager()
{
}

SingletonManager::~SingletonManager()
{
}

SingletonBase* SingletonManager::get(const char* name) const NOTHROW
{
    try
    {
        minitl::map< const char*, SingletonBase*, minitl::less<const char *> >::const_iterator it = m_registry.find(name);
        if(it != m_registry.end())
            return it->second;
        else
            return 0;
    }
    catch(...)
    {
        be_notreached();
        return 0;
    }
}

void SingletonManager::set(const char* name, SingletonBase* _instance)
{
    std::pair< minitl::map< const char*, SingletonBase*, minitl::less<const char *> >::iterator, bool > result = m_registry.insert(std::make_pair(name,_instance));
    be_assert(result.second, "could not register singleton for type %s" | name);
}

void SingletonManager::erase(const char* name, const SingletonBase* _instance)
{
    UNUSED(_instance);
    minitl::map< const char*, SingletonBase*, minitl::less<const char *> >::const_iterator it = m_registry.find(name);
    be_assert(it != m_registry.end(), "could not erase singleton %s" | name);
    be_assert(it->second == _instance, "singleton %s instance doesn't match registered instance" | name);
    (void)m_registry.erase(name);
}

SingletonManager* SingletonManager::instance() NOTHROW
{
    return &gInstance;
}

}
