/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
        Assert(gInstance);
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
