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
#include    <core/debug/memory/tag.hh>
#include    <core/debug/memory/location.hh>
#include    <core/threads/mutex.hh>

namespace BugEngine { namespace Memory
{

static size_t hashPointer(void* ptr)
{
    return (size_t)(ptr) >> 2;
}


class MemoryTag::Storage
{
public:
    static const size_t g_tagCapacity;
    i_u32               tagCount;
    MemoryTag*          tagBuffer;
    i_u32               maxTries;
public:
    Storage();
    ~Storage();
};

const size_t MemoryTag::Storage::g_tagCapacity = 1024*1024*2;

MemoryTag::Storage::Storage()
:   tagCount(0)
,   tagBuffer(Malloc::systemAllocArray<MemoryTag>(g_tagCapacity))
,   maxTries(0)
{
}

MemoryTag::Storage::~Storage()
{
    Malloc::systemFree(tagBuffer);
}

MemoryTag::Storage& MemoryTag::getStorage()
{
    static Storage s_storage;
    return s_storage;
}

MemoryTag::MemoryTag(void* memory, size_t size)
{
    m_info.memory = memory;
    m_info.size = size;
    m_info.location->add(this);
}

MemoryTag::~MemoryTag()
{
    m_info.location->remove(this);
    m_info.memory = 0;
}

bool MemoryTag::peek(MemoryTag::MemoryTagInfo& i, LocationInfo* owner) const
{
    const void* m = m_info.memory;
    if(owner && m_info.location != owner)
        return false;
    i = m_info;
    if(m_info.memory != m)
        return false;
    return true;
}

MemoryTag* MemoryTag::create(void* memory, size_t size, LocationInfo* location)
{
    MemoryTag* target = reserve(memory, location);
    new(target) MemoryTag(memory, size);
    return target;
}

MemoryTag* MemoryTag::reserve(void* memory, LocationInfo* owner)
{
    size_t index = hashPointer(memory);
    u32 tries = 0;
    Storage& s = getStorage();
    while(s.tagBuffer[index & (s.g_tagCapacity-1)].m_info.memory.setConditional(memory, 0) != 0)
    {
        index++;
        tries++;
        if(tries > s.maxTries)
            s.maxTries.setConditional(tries, s.maxTries);
    }
    s.tagBuffer[index & (s.g_tagCapacity-1)].m_info.location = owner;
    s.tagCount++;
    return &s.tagBuffer[index & (s.g_tagCapacity-1)];
}

bool MemoryTag::release(void* memory, MemoryTagInfo& copy)
{
    MemoryTag* t = find(memory);
    if(!t)
        return false;
    copy = t->m_info;
    t->~MemoryTag();
    getStorage().tagCount--;
    return true;
}

MemoryTag* MemoryTag::find(void* memory)
{
    size_t index = hashPointer(memory);
    size_t tries = 0;
    Storage& s = getStorage();
    while(s.tagBuffer[index & (s.g_tagCapacity-1)].m_info.memory != memory)
    {
        index++;
        tries++;
        if(tries > s.maxTries)
            return 0;
    }
    return &s.tagBuffer[index & (s.g_tagCapacity-1)];
}

MemoryTag* MemoryTag::begin()
{
    return getStorage().tagBuffer;
}

MemoryTag* MemoryTag::end()
{
    return getStorage().tagBuffer+getStorage().g_tagCapacity;
}

size_t  MemoryTag::usage()
{
    return getStorage().tagCount;
}

}}

