/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/istring.hh>
#include    <core/threads/criticalsection.hh>
#include    <core/memory/allocators/system.hh>
#include    <minitl/algorithm.hh>
#include    <minitl/hash_map.hh>
#include    <cstring>

namespace minitl
{

template< >
struct hash<const char*>
{
    static inline u32 get16bits(const u8 d[])
    {
        return (static_cast<u32>((d)[1]) << 8) | (static_cast<u32>((d)[0]));
    }
    u32 operator()(const char *str) const
    {
        const u8 *data = reinterpret_cast<const u8 *>(str);
        u32 len = str ? (u32)strlen(str) : 0;
        u32 hashvalue = len, tmp;
        u32 rem;
        if (len == 0 || data == NULL) return 0;

        rem = len & 3;
        len >>= 2;

        for (;len > 0; len--)
        {
            hashvalue  += get16bits (data);
            tmp         = (get16bits (data+2) << 11) ^ hashvalue;
            hashvalue   = (hashvalue << 16) ^ tmp;
            data       += 2*sizeof (u16);
            hashvalue  += hashvalue >> 11;
        }

        switch (rem) {
            case 3: hashvalue += get16bits (data);
                    hashvalue ^= hashvalue << 16;
                    hashvalue ^= static_cast<u32>(data[sizeof (u16)]) << 18;
                    hashvalue += hashvalue >> 11;
                    break;
            case 2: hashvalue += get16bits (data);
                    hashvalue ^= hashvalue << 11;
                    hashvalue += hashvalue >> 17;
                    break;
            case 1: hashvalue += *data;
                    hashvalue ^= hashvalue << 10;
                    hashvalue += hashvalue >> 1;
                    break;
            case 0:
            default:
                    break;
        }

        hashvalue ^= hashvalue << 3;
        hashvalue += hashvalue >> 5;
        hashvalue ^= hashvalue << 4;
        hashvalue += hashvalue >> 17;
        hashvalue ^= hashvalue << 25;
        hashvalue += hashvalue >> 6;

        return hashvalue;
    }
    bool operator()(const char *str1, const char *str2) const
    {
        return strcmp(str1, str2) == 0;
    }
};

}

namespace BugEngine
{

namespace Arena
{
static inline SystemAllocator& string()
{
    static SystemAllocator allocator(SystemAllocator::BlockSize_256k, 5);
    return allocator;
}
}

class StringInfo
{
private:
    typedef minitl::hashmap< const char *, u32 > StringIndex;

    class StringInfoBufferCache;
    class StringInfoBuffer
    {
        friend class StringInfoBufferCache;
    private:
        i_u32               m_used;
        byte                m_data[1];
    private:
        static u32 capacity();
    public:
        StringInfoBuffer();
        ~StringInfoBuffer();

        byte* reserve(u32 size);
    };

    class StringInfoBufferCache
    {
    private:
        iptr<StringInfoBuffer>* m_buffers;
        i_u32                   m_bufferCount;
    private:
        StringInfoBuffer* allocate(u32 place);
    public:
        StringInfoBufferCache();
        ~StringInfoBufferCache();
        StringInfo* resolve(u32 index);
        StringInfoBuffer* get(u32 index);

        u32 allocateStringInfo(u32 size);
    };

public:
    static StringInfoBufferCache& getCache();
    static u32 unique(const char *val);
    static u32 unique(const char *begin, const char *end);

private:
    mutable i_u32   m_refCount;
    u32             m_length;
    char            m_text[1];

private:
    StringInfo(u32 len)
        :   m_refCount(i_u32::Zero)
        ,   m_length(len)
        ,   m_text()
    {
    }
    StringInfo();
public:
    void retain(void) const     { m_refCount++; }
    void release(void) const    { be_assert(m_refCount, "string's refcount already 0"); m_refCount--; }
    size_t size() const         { return m_length; }
    const char *str() const     { return m_text; }
};

StringInfo::StringInfoBuffer::StringInfoBuffer()
    :   m_used(i_u32::Zero)
{
}

StringInfo::StringInfoBuffer::~StringInfoBuffer()
{
}

u32 StringInfo::StringInfoBuffer::capacity()
{
    return Arena::string().blockSize() - sizeof(StringInfoBuffer) + sizeof(byte[1]);
}

byte* StringInfo::StringInfoBuffer::reserve(u32 size)
{
    u32 allocated = (m_used += size) - size;
    if (allocated + size < capacity())
    {
        return m_data + allocated;
    }
    else
    {
        return 0;
    }
}

StringInfo::StringInfoBufferCache& StringInfo::getCache()
{
    static StringInfoBufferCache s_cache;
    return s_cache;
}

StringInfo::StringInfoBufferCache::StringInfoBufferCache()
    :   m_buffers(reinterpret_cast< iptr<StringInfo::StringInfoBuffer>* >(Arena::string().allocate()))
    ,   m_bufferCount(i_u32::Zero)
{
    allocate(0);
}

StringInfo::StringInfoBufferCache::~StringInfoBufferCache()
{
    for (u32 i = 0; i < m_bufferCount; ++i)
    {
        m_buffers[m_bufferCount - i - 1]->~StringInfoBuffer();
        Arena::string().free(m_buffers[m_bufferCount - i - 1]);
    }
    Arena::string().free(m_buffers);
}

StringInfo* StringInfo::StringInfoBufferCache::resolve(u32 index)
{
    StringInfo::StringInfoBuffer* buffer = get(index >> 16);
    byte* address = &buffer->m_data[index & 0xffff];
    return reinterpret_cast<StringInfo*>(address);
}

StringInfo::StringInfoBuffer* StringInfo::StringInfoBufferCache::get(u32 index)
{
    if (index < m_bufferCount)
    {
        return m_buffers[index];
    }
    else
    {
        be_assert(false, "Asking for a buffer out of range; current buffer count: %d, requested buffer: %d" | m_bufferCount | index);
        return 0;
    }
}

StringInfo::StringInfoBuffer* StringInfo::StringInfoBufferCache::allocate(u32 place)
{
    StringInfoBuffer* buffer = reinterpret_cast< StringInfoBuffer* >(Arena::string().allocate());
    if (m_buffers[place].setConditional(buffer, 0) == 0)
    {
        new (buffer) StringInfoBuffer;
        m_bufferCount++;
        return buffer;
    }
    else
    {
        Arena::string().free(buffer);
        return m_buffers[place];
    }
}

u32 StringInfo::StringInfoBufferCache::allocateStringInfo(u32 size)
{
    StringInfo::StringInfoBuffer* buffer;
    byte* info;
    u32 place;
    do
    {
        place = m_bufferCount - 1;
        buffer = m_buffers[place];
        info = buffer->reserve(minitl::align(sizeof(StringInfo) + size, be_alignof(StringInfo)));
        if (!info)
        {
            allocate(place+1);
        }
    } while (!info);
    return place << 16 | be_checked_numcast<u16>(reinterpret_cast<byte*>(info) - buffer->m_data);
}

u32 StringInfo::unique(const char *begin, const char *end)
{
    size_t _size = 1+size_t(end)-size_t(begin);
    char *val = static_cast<char*>(malloca(_size));
    strncpy(val,begin,_size-1);
    val[end-begin] = 0;
    u32 result = unique(val);
    freea(val);
    return result;
}

u32 StringInfo::unique(const char *val)
{
    static CriticalSection s_lock;
    ScopedCriticalSection scope(s_lock);
    static StringIndex g_strings(Arena::general(), 65536);
    StringIndex::iterator it = g_strings.find(val);
    if (it != g_strings.end())
    {
        return it->second;
    }
    else
    {
        u32 len = be_checked_numcast<u32>(strlen(val));
        u32 result = getCache().allocateStringInfo(len);
        StringInfo* cache = getCache().resolve(result);
        (void)(new(cache) StringInfo(len));
        strncpy(cache->m_text, val, len);
        cache->m_text[len] = 0;

        minitl::tuple<StringIndex::iterator,bool> insertresult = g_strings.insert(cache->m_text, result);
        be_forceuse(insertresult);
        return result;
    }
}

//-----------------------------------------------------------------------------

u32 istring::init(const char *str)
{
    u32 result = StringInfo::unique(str);
    StringInfo::getCache().resolve(result)->retain();
    return result;
}

u32 istring::init(const char *begin, const char *end)
{
    u32 result = StringInfo::unique(begin, end);
    StringInfo::getCache().resolve(result)->retain();
    return result;
}

istring::istring()
:   m_index(init(""))
{
}

istring::istring(const char *value)
:   m_index(init(value))
{
}

istring::istring(const char *begin, const char *end)
:   m_index(init(begin,end))
{
}

istring::istring(const istring& other)
:   m_index(other.m_index)
{
    StringInfo::getCache().resolve(m_index)->retain();
}

istring::~istring()
{
    StringInfo::getCache().resolve(m_index)->release();
}

istring& istring::operator=(const istring& other)
{
    if (&other == this)
        return *this;
    StringInfo::getCache().resolve(other.m_index)->retain();
    StringInfo::getCache().resolve(m_index)->release();
    m_index = other.m_index;
    return *this;
}

u32 istring::hash() const
{
    return (u32)(ptrdiff_t(m_index) & 0xFFFFFFFF);
}

size_t istring::size() const
{
    return StringInfo::getCache().resolve(m_index)->size();
}

const char *istring::c_str() const
{
    return StringInfo::getCache().resolve(m_index)->str();
}

bool istring::operator==(const istring& other) const
{
    return other.m_index == m_index;
}

bool istring::operator!=(const istring& other) const
{
    return other.m_index != m_index;
}

bool istring::operator<(const istring& other) const
{
    return m_index < other.m_index;
}

//-----------------------------------------------------------------------------
static inline const char* findnext(const char* str, const char *sep, size_t len)
{
    const char *result = str;
    while (*result)
    {
        for (size_t i = 0; i < len && *result; ++i)
            if (*result == sep[i])
                return result;
        result++;
    }
    return result;
}

static void parse(const char *str, const char *end, const char *sep, igenericnamespace& result)
{
    size_t numsep = strlen(sep);

    if (str && *str)
    {
        const char *ss = str;
        while (*ss && ss < end)
        {
            ss = findnext(str, sep, numsep);
            result.push_back(istring(str,ss));
            if (*ss) ss++;
            str=ss;
        }
    }
    return;
}

igenericnamespace::igenericnamespace()
    :   m_namespace((istring*)(m_buffer))
    ,   m_size(0)
    ,   m_capacity(MaxNamespaceSize)
{
}

igenericnamespace::igenericnamespace(const igenericnamespace& other)
    :   m_namespace(other.m_capacity > MaxNamespaceSize
                        ?   (istring*)Arena::general().alloc(other.m_capacity * sizeof(istring))
                        :   (istring*)(m_buffer))
    ,   m_size(other.m_size)
    ,   m_capacity(other.m_capacity)
{
    for (u16 i = 0; i < m_size; ++i)
    {
        new (&m_namespace[i]) istring(other.m_namespace[i]);
    }
}

igenericnamespace::igenericnamespace(const istring& onlycomponent)
    :   m_namespace((istring*)(m_buffer))
    ,   m_size(1)
    ,   m_capacity(MaxNamespaceSize)
{
    new (&m_namespace[0]) istring(onlycomponent);
}

igenericnamespace::igenericnamespace(const char *begin, const char *end, const char* sep)
    :   m_namespace((istring*)(m_buffer))
    ,   m_size(0)
    ,   m_capacity(MaxNamespaceSize)
{
    parse(begin, end, sep, *this);
}

igenericnamespace::igenericnamespace(const char *str, const char* sep)
    :   m_namespace((istring*)(m_buffer))
    ,   m_size(0)
    ,   m_capacity(MaxNamespaceSize)
{
    if (str)
    {
        parse(str, str+strlen(str), sep, *this);
    }
}

igenericnamespace& igenericnamespace::operator=(const igenericnamespace& other)
{
    if (&other != this)
    {
        for (u16 i = 0; i < m_size; ++i)
        {
            m_namespace[m_size-i-1].~istring();
        }
        m_size = 0;
        if (m_capacity < other.m_size)
        {
            grow(other.m_size);
        }
        for (u16 i = 0; i < other.m_size; ++i)
        {
            new (&m_namespace[i]) istring(other.m_namespace[i]);
        }
        m_size = other.m_size;
    }
    return *this;
}

igenericnamespace::~igenericnamespace()
{
    for (u16 i = 0; i < m_size; ++i)
    {
        m_namespace[m_size-i-1].~istring();
    }
    if (m_capacity > MaxNamespaceSize)
    {
        Arena::general().free(m_namespace);
    }
}

u32 igenericnamespace::size() const
{
    return m_size;
}

const istring& igenericnamespace::operator[](u32 index) const
{
    be_assert(index < m_size, "index %d out of range %d" | index | m_size);
    return m_namespace[index];
}

void igenericnamespace::push_back(const istring& component)
{
    if (m_size == m_capacity)
    {
        grow(m_capacity * 2);
    }
    new (&m_namespace[m_size++]) istring(component);
}

istring igenericnamespace::pop_back()
{
    be_assert_recover(m_size >= 1, "pop_back called on an empty namespace", return istring());
    --m_size;
    istring result = m_namespace[m_size];
    m_namespace[m_size].~istring();
    return result;
}

istring igenericnamespace::pop_front()
{
    be_assert_recover(m_size >= 1, "pop_front called on an empty namespace", return istring());
    istring result = m_namespace[0];
    for (u32 i = 1; i < m_size; ++i)
        m_namespace[i-1] = m_namespace[i];
    m_size--;
    m_namespace[m_size].~istring();
    return result;
}

bool igenericnamespace::operator==(const igenericnamespace& other) const
{
    if (this->size() != other.size())
        return false;
    for (u32 i = 0; i < other.size(); ++i)
        if (m_namespace[i] != other[i])
            return false;
    return true;
}

bool igenericnamespace::operator!=(const igenericnamespace& other) const
{
    if (this->size() != other.size())
        return true;
    for (u32 i = 0; i < other.size(); ++i)
        if (m_namespace[i] != other[i])
            return true;
    return false;
}

bool startswith(const igenericnamespace& start, const igenericnamespace& full)
{
    for (u32 i = 0; i < start.size(); ++i)
        if (start[i] != full[i])
            return false;
    return true;
}

bool operator<(const igenericnamespace& ns1, const igenericnamespace& ns2)
{
    for (u32 i = 0; i < minitl::min(ns1.size(), ns2.size()); ++i)
    {
        if (ns1[i] < ns2[i])
            return true;
        else if (ns2[i] < ns1[i])
            return false;
    }
    return ns1.size() < ns2.size();
}

void igenericnamespace::str(char* buffer, char separator, u32 size) const
{
    buffer[0] = 0;
    buffer[size-1] = 0;
    char sep[2] = {separator, '\0'};
    if (m_size > 0)
    {
        strncpy(buffer, m_namespace[0].c_str(), size-1);
        for (u32 i = 1; i < m_size; ++i)
        {
            strncat(buffer, sep, size - strlen(buffer) - 1);
            strncat(buffer, m_namespace[i].c_str(), size - strlen(buffer) - 1);
        }
    }
}

void igenericnamespace::grow(u16 newCapacity)
{
    istring* newStrings = (istring*)Arena::general().alloc(newCapacity * sizeof(istring));
    for (u16 i = 0; i < m_size; ++i)
        new (&newStrings[i]) istring(m_namespace[i]);
    for (u16 i = m_size; i < newCapacity; ++i)
        new (&newStrings[i]) istring;
    for (u16 i = 0; i < m_size; ++i)
    {
        m_namespace[m_size-i-1].~istring();
    }
    if (m_capacity > MaxNamespaceSize)
    {
        Arena::general().free(m_namespace);
    }
    minitl::swap(newStrings, m_namespace);
    m_capacity = newCapacity;
}

//-----------------------------------------------------------------------------

inamespace::inamespace()
{
}

inamespace::inamespace(const istring& onlycomponent)
:   igenericnamespace(onlycomponent)
{
}

inamespace::inamespace(const char* _str)
:   igenericnamespace(_str, ".")
{
}

inamespace& inamespace::operator+=(const istring& str2)
{
    return this->operator +=(inamespace(str2));
}

inamespace& inamespace::operator+=(const inamespace& str2)
{
    for (u32 i = 0; i < str2.size(); ++i)
        push_back(str2[i]);
    return *this;
}

inamespace operator+(const istring& str1, const istring& str2)
{
    return inamespace(str1)+inamespace(str2);
}

inamespace operator+(const istring& str1, const inamespace& ns2)
{
    return inamespace(str1)+ns2;
}

inamespace operator+(const inamespace& ns1, const istring& str2)
{
    return ns1+inamespace(str2);
}

inamespace operator+(const inamespace& ns1, const inamespace& ns2)
{
    inamespace result = ns1;
    result += ns2;
    return result;
}

inamespace::Path inamespace::str() const
{
    Path p;
    igenericnamespace::str(p.name, Separator, sizeof(p.name));
    return p;
}

//-----------------------------------------------------------------------------

ipath::ipath()
{
}

ipath::ipath(const istring& onlycomponent)
:   igenericnamespace(onlycomponent)
{
}

ipath::ipath(const char *_str)
:   igenericnamespace(_str, "/\\")
{
}

ipath::ipath(const char *begin, const char *end)
:   igenericnamespace(begin, end, "/\\")
{
}

ipath& ipath::operator+=(const ipath& str2)
{
    for (u32 i = 0; i < str2.size(); ++i)
        push_back(str2[i]);
    return *this;
}

ipath operator+(const ipath& path1, const ipath& path2)
{
    ipath result = path1;
    result += path2;
    return result;
}

ipath::Filename ipath::str() const
{
    Filename p;
    igenericnamespace::str(p.name, Separator, sizeof(p.name));
    return p;
}

//-----------------------------------------------------------------------------

ifilename::ifilename()
{
}

ifilename::ifilename(const istring& onlycomponent)
:   igenericnamespace(onlycomponent)
{
}

ifilename::ifilename(const ipath& path)
:   igenericnamespace(path)
{
}

ifilename::ifilename(const char *_str)
:   igenericnamespace(_str, "/\\")
{
}

ifilename operator+(const ipath& path, const ifilename& file)
{
    ifilename result("");
    for (u32 i = 0; i < path.size(); ++i)
        result.push_back(path[i]);
    for (u32 i = 0; i < file.size(); ++i)
        result.push_back(file[i]);
    return result;
}

ifilename::Filename ifilename::str() const
{
    Filename p;
    igenericnamespace::str(p.name, Separator, sizeof(p.name));
    return p;
}

}
