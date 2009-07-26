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
#include    <core/debug/memory/location.hh>
#include    <core/debug/memory/tag.hh>
#include    <core/debug/callstack.hh>
#include    <core/threads/mutex.hh>

#ifndef _WIN32
static int strnicmp(const char *s1, const char *s2, size_t len)
{
    unsigned char c1, c2;

    c1 = c2 = 0;
    while(len--)
    {
        c1 = *s1;
        c2 = *s2;
        s1++;
        s2++;
        if (!c1)
            break;
        if (!c2)
            break;
        if (c1 == c2)
            continue;
        c1 = tolower(c1);
        c2 = tolower(c2);
        if (c1 != c2)
            break;
    }
    return (int)c1 - (int)c2;
}
#endif

#ifdef BE_PLATFORM_WIN32
# define debugPrint(msg)    OutputDebugString((msg))
#else
# define debugPrint(msg)    printf("%s",(msg))
#endif


namespace BugEngine
{


static size_t hashPointer(void* ptr)
{
    return (size_t)(ptr) >> 2;
}



class LocationInfo::Storage
{
private:
    template< typename T >
    class LinearAllocator
    {
    private:
        i_size_t    m_size;
        T*          m_buffer;
        size_t      m_capacity;
    public:
        LinearAllocator(size_t capacity)
            :   m_buffer(Malloc::systemAllocArray<T>(capacity))
            ,   m_size(0)
            ,   m_capacity(capacity)
        {
        }
        ~LinearAllocator()
        {
            Malloc::systemFree(m_buffer);
        }

        T* reserve(size_t count)
        {
            size_t offset = m_size.addExchange(count);
            AssertMsg(offset+count <= m_capacity, minitl::format<>("%u+%u > %u") | offset | count | m_capacity);
            return m_buffer + offset;
        }
    };
    struct FileRootInformation
    {
        size_t          size;
        const char *    prefix;
    };
private:
    static const size_t     g_locationCapacity          = 1024*256;
    static const size_t     g_adressTranslationCapacity = 1024*256;
    static const size_t     g_stringCapacity            = 4*1024*1024;
private:
    LinearAllocator<LocationInfo>   m_locationBuffer;
    LinearAllocator<char>           m_stringBuffer;
    LocationInfo*                   m_indexedLocations;
    i_u32                           m_lookupOffset;
    FileRootInformation             m_sourceRoots[3];
    LocationInfo*                   m_root;
public:
    Storage();
    ~Storage();

    const char *  storeString(const char *string, size_t size);
    LocationInfo* allocateLocation();
    LocationInfo* createLocation(const char *filename, const char *desc);
    LocationInfo* getLocationForAdress(const Callstack::Address& a);

    const LocationInfo* root() const;
    void update();
};

LocationInfo::Storage::Storage()
:   m_locationBuffer(g_locationCapacity)
,   m_stringBuffer(g_stringCapacity)
,   m_indexedLocations(Malloc::systemAllocArray<LocationInfo>(g_adressTranslationCapacity))
,   m_lookupOffset(0)
,   m_root(allocateLocation())
{
    m_sourceRoots[0].prefix = "..\\..";
    m_sourceRoots[0].size = strlen(m_sourceRoots[0].prefix);
    m_sourceRoots[1].prefix = BE_SOURCEDIR;
    m_sourceRoots[1].size = strlen(m_sourceRoots[1].prefix);
    m_sourceRoots[2].prefix = 0;
    m_sourceRoots[2].size = 0;
    new(m_root) LocationInfo;
}

static void dumpName(const LocationInfo* i)
{
    if(i->parent())
    {
        dumpName(i->parent());
        debugPrint("/");
    }
    debugPrint(i->filename);
}

LocationInfo::Storage::~Storage()
{
    debugPrint("-< ALLOCATED MEMORY >-\n");
    for(LocationInfo* i = m_indexedLocations; i < m_indexedLocations+g_adressTranslationCapacity;++i)
    {
        if(i->m_owner)
        {
            if(i->stats.aliveAllocs)
            {
                dumpName(i->parent());
                debugPrint((minitl::format<>("%s : %u blocks\n") | i->filename | i->stats.aliveAllocs).c_str());
            }
        }
    }
    debugPrint("-<        END       >-\n");
    Malloc::systemFree(m_indexedLocations);
}

const char * LocationInfo::Storage::storeString(const char *string, size_t size)
{
    char* result = m_stringBuffer.reserve(size+1);
    strncpy(result, string, size);
    result[size] = 0;
    return result;
}

LocationInfo* LocationInfo::Storage::allocateLocation()
{
    return m_locationBuffer.reserve(1);
}

LocationInfo* LocationInfo::Storage::createLocation(const char *filename, const char *desc)
{
    for(int i = 0; m_sourceRoots[i].prefix != 0; ++i)
    {
        if(strnicmp(filename, m_sourceRoots[i].prefix, m_sourceRoots[i].size) == 0)
        {
            filename = filename + m_sourceRoots[i].size+1;
            break;
        }
    }

    return m_root->insert(filename)->insert(desc);
}

LocationInfo* LocationInfo::Storage::getLocationForAdress(const Callstack::Address& address)
{
    size_t hash = hashPointer(address.pointer());

    for(size_t offset = hash; offset < hash+m_lookupOffset; ++offset)
    {
        if(m_indexedLocations[offset & (g_adressTranslationCapacity-1)].m_owner == address.pointer())
            return &m_indexedLocations[offset & (g_adressTranslationCapacity-1)];
    }

    u32 tries = 0;
    while(m_indexedLocations[ (hash+tries) & g_adressTranslationCapacity-1 ].m_owner.setConditional(address.pointer(), 0) != 0)
    {
        tries++;
    }
    while(tries+1 > m_lookupOffset)
    {
        m_lookupOffset.setConditional(tries+1, m_lookupOffset);
    }

    const char *file = address.filename();
    for(int i = 0; m_sourceRoots[i].prefix != 0; ++i)
    {
        if(strnicmp(file, m_sourceRoots[i].prefix, m_sourceRoots[i].size) == 0)
        {
            file = file + m_sourceRoots[i].size+1;
            break;
        }
    }

    const char* dupSegment;

    LocationInfo* parent = m_root->insert(file);
    parent = parent->insert(address.function());
    minitl::format<> line =  minitl::format<>("(%u)") | address.line();
    dupSegment = storeString(line, strlen(line)+1);

    LocationInfo* result = new(&m_indexedLocations[ (hash+tries) & (g_adressTranslationCapacity-1) ]) LocationInfo(parent, dupSegment);
    LocationInfo* child = parent->m_child;
    LocationInfo* prev  = 0;
    while(child && strcmp(child->filename, file) < 0)
    {
        prev = child;
        child = child->m_sibling;
    }
    parent->insert(result, prev);

    return result;
}

const LocationInfo* LocationInfo::Storage::root() const
{
    return m_root;
}

void LocationInfo::Storage::update()
{
    m_root->frameUpdate();
}



LocationInfo::Storage& LocationInfo::getStorage()
{
    static Storage s_storage;
    return s_storage;
}

LocationInfo::LocationInfo()
    :   filename("$")
    ,   m_parent(0)
    ,   m_sibling(0)
    ,   m_child(0)
    ,   m_flags(0)
{
}

LocationInfo::LocationInfo(LocationInfo* parent, const char *file)
    :   filename(file)
    ,   m_parent(parent)
    ,   m_sibling(0)
    ,   m_child(0)
    ,   m_flags(0)
{
}

LocationInfo::~LocationInfo()
{
}

LocationInfo* LocationInfo::location(const Callstack::Address& address)
{
    return getStorage().getLocationForAdress(address);
}

LocationInfo* LocationInfo::location(const char* filename, const char* desc)
{
    return getStorage().createLocation(filename, desc);
}

void LocationInfo::insert(LocationInfo* child, LocationInfo* left)
{
    if(! left)
    {
        child->m_sibling = this->m_child;
        this->m_child = child;
    }
    else
    {
        child->m_sibling = left->m_sibling;
        left->m_sibling = child;
    }
}

LocationInfo* LocationInfo::insert(const char* file)
{
    const char* segment = file;
    while(*segment != '\0' && *segment != '/' && *segment != '\\')
        segment++;
    size_t segmentLength = segment-file;

    static Mutex s_locationLock;
    ScopedMutexLock lock(s_locationLock);

    LocationInfo *child = this->m_child;
    LocationInfo* prev  = 0;
    while(child && strnicmp(child->filename, file, segmentLength) < 0)
    {
        prev = child;
        child = child->m_sibling;
    }

    if(!child || strnicmp(child->filename, file, segmentLength) > 0)
    {
        Storage& s = getStorage();
        LocationInfo* i = s.allocateLocation();
        const char* dupSegment = s.storeString(file, segment-file);
        new((void*)i) LocationInfo(this, dupSegment);
        insert(i, prev);
        if(!*segment)
            return i;
        else
            return i->insert(segment+1);
    }
    else if(!*segment)
    {
        return child;
    }
    else
    {
        return child->insert(segment+1);
    }
}


void LocationInfo::add(Memory::MemoryTag *tag)
{
    stats.thisFrameAllocs++;
    stats.aliveAllocs++;
    stats.size += tag->m_info.size;
    if(isWatched())
        BREAKPOINT;
    if(m_parent)
        m_parent->add(tag);
}

void LocationInfo::remove(Memory::MemoryTag *tag)
{
    stats.aliveAllocs--;
    stats.size -= tag->m_info.size;
    if(isWatched())
        BREAKPOINT;
    if(m_parent)
        m_parent->remove(tag);
}

const LocationInfo* LocationInfo::root()
{
    return getStorage().root();
}

void LocationInfo::frameUpdate()
{
    getStorage().update();
}

void LocationInfo::_frameUpdate()
{
    stats.lastFrameAllocs = stats.thisFrameAllocs.exchange(0);
    if(isExpanded())
    {
        LocationInfo* child = this->m_child;
        while(child)
        {
            child->_frameUpdate();
            child = child->m_sibling;
        }
    }
}

bool LocationInfo::isWatched() const
{
    return (m_flags & FlagBreakpoint) != 0;
}

void LocationInfo::toggleWatched() const
{
    m_flags ^= FlagBreakpoint;
}

bool LocationInfo::isExpanded() const
{
    return (m_flags & FlagExpanded) != 0;
}

void LocationInfo::toggleExpanded() const
{
    m_flags ^= FlagExpanded;
}

}

