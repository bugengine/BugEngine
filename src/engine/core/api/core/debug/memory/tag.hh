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

#ifndef BE_CORE_DEBUG_MEMORYTAG_HH_
#define BE_CORE_DEBUG_MEMORYTAG_HH_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked>

namespace BugEngine
{
class Malloc;
class LocationInfo;
}

namespace BugEngine { namespace Memory
{

class MemoryTag
{
    friend class ::BugEngine::Malloc;
    friend class ::BugEngine::LocationInfo;
public:
    struct MemoryTagInfo
    {
        LocationInfo*       location;
        minitl::iptr<void>  memory;
        size_t              size;
    };
private:
    class Storage;
    static Storage& getStorage();
private:
    MemoryTagInfo   m_info;
private:
    MemoryTag(void* memory, size_t size);
    ~MemoryTag();
public:
    static COREEXPORT MemoryTag*   create(void* memory, size_t size, LocationInfo* location);
    static COREEXPORT MemoryTag*   reserve(void* memory, LocationInfo *lock);
    static COREEXPORT MemoryTag*   find(void* memory);
    static COREEXPORT bool         release(void* memory, MemoryTagInfo& target);
public:
    static COREEXPORT MemoryTag*   begin();
    static COREEXPORT MemoryTag*   end();
    static COREEXPORT size_t       usage();

    bool peek(MemoryTag::MemoryTagInfo& i, LocationInfo* owner = 0) const;
};

}}

/*****************************************************************************/
#endif

