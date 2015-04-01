/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/istring.hh>
#include    <core/threads/criticalsection.hh>
#include    <minitl/algorithm.hh>
#include    <minitl/hash_map.hh>
#include    <cstring>

namespace BugEngine
{

namespace Arena
{
static inline minitl::Allocator& string()
{
    return general();
}
}

class StringCache
{
private:
    class Buffer
    {
    private:
        Buffer*                         m_next;
        minitl::Allocator::Block<byte>  m_buffer;
        i_size_t                        m_used;
        static const size_t             s_capacity = 1024*200;
    private:
        StringCache*    reserveNext(size_t size);
    public:
        Buffer();
        ~Buffer();

        StringCache*        reserve(size_t size);
    };
private:
    typedef minitl::hashmap< const char *, StringCache* > StringIndex;
private:
    static Buffer*  getBuffer();
public:
    static StringCache* unique(const char *val);
    static StringCache* unique(const char *begin, const char *end);
private:
    mutable i_u32  m_refCount;
    u32  m_length;
#   ifdef  BE_DEBUG
    const char *m_text;
    size_t  m_gard;
#   endif
private:
    StringCache(u32 len)
        :   m_refCount(i_u32::Zero)
        ,   m_length(len)
#   ifdef  BE_DEBUG
        ,   m_text((const char *)(this+1))
        ,   m_gard(0xDEADBEEF)
#   endif
    {
    }
    StringCache();
public:
    void retain(void) const     { m_refCount++; }
    void release(void) const    { be_assert(m_refCount, "string's refcount already 0"); m_refCount--; }
    size_t size() const         { return m_length; }
    const char *str() const     { return reinterpret_cast<const char *>(this+1); }
};

StringCache::Buffer* StringCache::getBuffer()
{
    static Buffer s_root;
    return &s_root;
}

StringCache::Buffer::Buffer()
:   m_next(0)
,   m_buffer(Arena::string(), s_capacity)
,   m_used(i_size_t::Zero)
{
}

StringCache::Buffer::~Buffer()
{
    delete m_next;
}

StringCache* StringCache::Buffer::reserve(size_t size)
{
    size_t allsize = minitl::align(size+1+sizeof(StringCache), be_alignof(StringCache));
    be_assert(allsize < s_capacity, "string size is bigger than pool size");
    if (m_used > s_capacity)
    {
        return reserveNext(size);
    }
    size_t offset = m_used.addExchange(allsize);
    if (offset+allsize < s_capacity)
    {
        return (StringCache*)(m_buffer+offset);
    }
    else
    {
        return reserveNext(size);
    }
}

StringCache* StringCache::Buffer::reserveNext(size_t size)
{
    if (!m_next)
    {
        m_next = new Buffer;
    }
    return m_next->reserve(size);
}

StringCache* StringCache::unique(const char *begin, const char *end)
{
    size_t _size = 1+size_t(end)-size_t(begin);
    char *val = static_cast<char*>(malloca(_size));
    strncpy(val,begin,_size-1);
    val[end-begin] = 0;
    StringCache* result = unique(val);
    freea(val);
    return result;
}

StringCache* StringCache::unique(const char *val)
{
    static CriticalSection s_lock;
    ScopedCriticalSection scope(s_lock);
    static StringIndex g_strings(Arena::string(), 65536);
    StringIndex::iterator it = g_strings.find(val);
    if (it != g_strings.end())
    {
        return it->second;
    }
    else
    {
        u32 len = be_checked_numcast<u32>(strlen(val));
        StringCache* cache = getBuffer()->reserve(len);
        char *data = (char*)cache + sizeof(StringCache);

        (void)(new(cache) StringCache(len));
        strcpy(data, val);

        minitl::tuple<StringIndex::iterator,bool> insertresult = g_strings.insert(data, cache);
        be_forceuse(insertresult);
        return cache;
    }
}

//-----------------------------------------------------------------------------

const StringCache* istring::init(const char *str)
{
    StringCache* result = StringCache::unique(str);
    result->retain();
    return result;
}

const StringCache* istring::init(const char *begin, const char *end)
{
    StringCache* result = StringCache::unique(begin, end);
    result->retain();
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
    m_index->retain();
}

istring::~istring()
{
    m_index->release();
}

istring& istring::operator=(const istring& other)
{
    if (&other == this)
        return *this;
    other.m_index->retain();
    m_index->release();
    m_index = other.m_index;
    return *this;
}

u32 istring::hash() const
{
    return (u32)(ptrdiff_t(m_index) & 0xFFFFFFFF);
}

size_t istring::size() const
{
    return m_index->size();
}

const char *istring::c_str() const
{
    return m_index->str();
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
                        ?   (istring*)Arena::string().alloc(other.m_capacity * sizeof(istring))
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
    for (u16 i = 0; i < m_size; ++i)
    {
        new (&m_namespace[i]) istring(other.m_namespace[i]);
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
        Arena::string().free(m_namespace);
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
    void* newBuffer = Arena::string().alloc(newCapacity * sizeof(istring));
    istring* newStrings = new (newBuffer) istring[newCapacity];
    for (u16 i = 0; i < m_size; ++i)
        newStrings[i] = m_namespace[i];
    for (u16 i = 0; i < m_size; ++i)
    {
        m_namespace[m_size-i-1].~istring();
    }
    if (m_capacity > MaxNamespaceSize)
    {
        Arena::string().free(m_namespace);
    }
    minitl::swap(newStrings, m_namespace);
    m_capacity = newCapacity;
}

//-----------------------------------------------------------------------------

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

ifilename::ifilename(const istring& onlycomponent)
:   igenericnamespace(onlycomponent)
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
