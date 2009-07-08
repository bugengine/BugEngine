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

#ifndef BE_CORE_MALLOCDEBUG_HH_
#define BE_CORE_MALLOCDEBUG_HH_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked>
#include    <core/debug/callstack.hh>

namespace BugEngine
{

namespace Memory
{
class MemoryTag;
}

struct LocationStatistics
{
    i_size_t    size;
    i_u32       aliveAllocs;
    i_u32       lastFrameAllocs;
    i_u32       thisFrameAllocs;
};

class LocationInfo
{
private:
    enum
    {
        FlagBreakpoint = 1 << 0,
        FlagExpanded   = 1 << 1
    };
private:
    class Storage;
    static Storage& getStorage();
private:
    LocationInfo();
    LocationInfo(LocationInfo* parent, const char* file);
    COREEXPORT ~LocationInfo();

    LocationInfo*   insert(const char* file);
    void            insert(LocationInfo* child, LocationInfo* left);

    void            _frameUpdate();
public:
    static COREEXPORT LocationInfo* location(const Callstack::Address& from);
    static COREEXPORT LocationInfo* location(const char* filename, const char *desc);

    void add(Memory::MemoryTag *tag);
    void remove(Memory::MemoryTag *tag);

    static void frameUpdate();

    COREEXPORT bool isWatched() const;
    COREEXPORT void toggleWatched() const;
    COREEXPORT bool isExpanded() const;
    COREEXPORT void toggleExpanded() const;

    static COREEXPORT const LocationInfo* root();

    const LocationInfo* parent() const { return m_parent; }
    const LocationInfo* child() const { return m_child; }
    const LocationInfo* sibling() const { return m_sibling; }
private:
    minitl::iptr<void>  m_owner;
public:
    const char * const  filename;
    LocationStatistics  stats;
private:
    LocationInfo* const m_parent;
    LocationInfo*       m_child;
    LocationInfo*       m_sibling;
private:
    mutable size_t      m_flags;

private:
    LocationInfo& operator=(const LocationInfo& other);
    LocationInfo(const LocationInfo& other);
};

}

/*****************************************************************************/
#endif

