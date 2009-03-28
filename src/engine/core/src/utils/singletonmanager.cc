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
        AssertNotReached();
        return 0;
    }
}

void SingletonManager::set(const char* name, SingletonBase* _instance)
{
    std::pair< minitl::map< const char*, SingletonBase*, minitl::less<const char *> >::iterator, bool > result = m_registry.insert(std::make_pair(name,_instance));
    Assert(result.second);
}

void SingletonManager::erase(const char* name, const SingletonBase* _instance)
{
    UNUSED(_instance);
    minitl::map< const char*, SingletonBase*, minitl::less<const char *> >::const_iterator it = m_registry.find(name);
    Assert(it != m_registry.end());
    Assert(it->second == _instance);
    (void)m_registry.erase(name);
}

SingletonManager* SingletonManager::instance() NOTHROW
{
    return &gInstance;
}

}
