/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/istring.hh>
#include    <core/threads/criticalsection.hh>
#include    <minitl/container/algorithm.hh>
#include    <cstring>


namespace BugEngine
{

static inline Allocator& stringArena()
{
    return gameArena();
}

struct hashIstring
{
    bool operator()(const char *str1, const char *str2) const
    {
        return strcmp(str1, str2) < 0;
    }
};


class StringCache
{
    friend class StringInit;
private:
    class Buffer
    {
    private:
        Buffer*                 m_next;
        Allocator::Block<byte>  m_buffer;
        i_size_t                m_used;
        static const size_t     s_capacity = 1024*200;
    private:
        StringCache*    reserveNext(size_t size);
    public:
        Buffer();
        ~Buffer();

        StringCache*        reserve(size_t size);
    };
private:
    friend struct hashWithOffset;
    friend struct lessWithOffset;
    friend struct equaltoWithOffset;
private:
    typedef minitl::hashmap< const char *, StringCache*, hashIstring > StringIndex;
private:
    static Buffer*  getBuffer();
public:
    static StringCache* unique(const char *val);
    static StringCache* unique(const char *begin, const char *end);
private:
    mutable i_u32  m_refCount;
    size_t  m_hash;
    size_t  m_length;
#   ifdef  BE_DEBUG
    size_t  m_gard;
#   endif
private:
    StringCache(size_t _hash, size_t len) :
        m_refCount(0),
        m_hash(_hash),
        m_length(len)
#   ifdef  BE_DEBUG
        ,m_gard(0xDEADBEEF)
#   endif
    {
    }
    StringCache();
public:
    void retain(void) const     { m_refCount++; }
    void release(void) const    { be_assert(m_refCount, "string's refcount already 0"); m_refCount--; }
    u64 hash() const            { return m_hash; }
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
,   m_buffer(stringArena(), s_capacity)
,   m_used(0)
{
}

StringCache::Buffer::~Buffer()
{
    delete m_next;
}

StringCache* StringCache::Buffer::reserve(size_t size)
{
    size_t allsize = be_align(size+1+sizeof(StringCache), be_alignof(StringCache));
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
    static StringIndex g_strings(stringArena());
    StringIndex::iterator it = g_strings.find(val);
    if (it != g_strings.end())
    {
        return it->second;
    }
    else
    {
        size_t len = strlen(val);
        StringCache* cache = getBuffer()->reserve(len);
        char *data = (char*)cache + sizeof(StringCache);

        size_t hashval = minitl::str_hash(val,len);
        (void)(new(cache) StringCache(hashval, len));
        strcpy(data, val);

        std::pair<StringIndex::iterator,bool> insertresult = g_strings.insert(std::make_pair((const char*)data, cache));
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
#ifdef BE_DEBUG
,   m_str(m_index->str())
#endif
{
}

istring::istring(const char *value)
:   m_index(init(value))
#ifdef BE_DEBUG
,   m_str(m_index->str())
#endif
{
}

istring::istring(const char *begin, const char *end)
:   m_index(init(begin,end))
#ifdef BE_DEBUG
,   m_str(m_index->str())
#endif
{
}

istring::istring(const istring& other)
:   m_index(other.m_index)
#ifdef BE_DEBUG
,   m_str(other.m_str)
#endif
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
#ifdef BE_DEBUG
    m_str = other.m_str;
#endif
    return *this;
}

u64 istring::hash() const
{
    return m_index->hash();
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

static void parse(const char *str, const char *end, const char *sep, istring* buffer, u32& size)
{
    size_t numsep = strlen(sep);

    if (str && *str)
    {
        const char *ss = str;
        while (*ss && ss < end)
        {
            ss = findnext(str, sep, numsep);
            buffer[size++] = istring(str,ss);
            if (*ss) ss++;
            str=ss;
        }
    }
    return;
}

igenericnamespace::igenericnamespace()
:   m_namespace()
,   m_size(0)
{
}

igenericnamespace::igenericnamespace(const istring& onlycomponent)
:   m_namespace()
,   m_size(1)
{
    m_namespace[0] = onlycomponent;
}

igenericnamespace::igenericnamespace(const char *begin, const char *end, const char* sep)
:   m_namespace()
,   m_size(0)
{
    parse(begin, end, sep, m_namespace, m_size);
}

igenericnamespace::igenericnamespace(const char *str, const char* sep)
:   m_namespace()
,   m_size(0)
{
    parse(str, str+strlen(str), sep, m_namespace, m_size);
}

size_t igenericnamespace::size() const
{
    return m_size;
}

const istring& igenericnamespace::operator[](size_t index) const
{
    be_assert(m_size > index, "index %d out of range %d" | index | m_size);
    return m_namespace[index];
}

void igenericnamespace::push_back(const istring& component)
{
    be_assert(m_size < MaxNamespaceSize, "index %d too large; max size is %d" | m_size | MaxNamespaceSize);
    m_namespace[m_size++] = component;
}

istring igenericnamespace::pop_back()
{
    --m_size;
    return m_namespace[m_size];
}

istring igenericnamespace::pop_front()
{
    istring i = m_namespace[0];
    for (size_t i = 1; i < m_size; ++i)
        m_namespace[i-1] = m_namespace[i];
    m_size--;
    return i;
}

bool igenericnamespace::operator==(const igenericnamespace& other) const
{
    if ( this->size() != other.size())
        return false;
    for (size_t i = 0; i < other.size(); ++i)
        if (m_namespace[i] != other[i])
            return false;
    return true;
}

bool igenericnamespace::operator!=(const igenericnamespace& other) const
{
    if ( this->size() != other.size())
        return true;
    for (size_t i = 0; i < other.size(); ++i)
        if (m_namespace[i] != other[i])
            return true;
    return false;
}

bool startswith(const igenericnamespace& start, const igenericnamespace& full)
{
    for (size_t i = 0; i < start.size(); ++i)
        if (start[i] != full[i])
            return false;
    return true;
}

bool operator<(const igenericnamespace& ns1, const igenericnamespace& ns2)
{
    for (size_t i = 0; i < minitl::min(ns1.size(), ns2.size()); ++i)
    {
        if (ns1[i] < ns2[i])
            return true;
        else if (ns2[i] < ns1[i])
            return false;
    }
    return ns1.size() < ns2.size();
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

minitl::format<inamespace::MaxNamespaceLength> inamespace::str() const
{
    return igenericnamespace::tostring<inamespace::MaxNamespaceLength>(".");
}

inamespace& inamespace::operator+=(const istring& str2)
{
    return this->operator +=(inamespace(str2));
}

inamespace& inamespace::operator+=(const inamespace& str2)
{
    for (size_t i = 0; i < str2.size(); ++i)
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

minitl::format<ipath::MaxFilenameLength> ipath::str() const
{
#   ifdef BE_PLATFORM_WIN32
    return igenericnamespace::tostring<ipath::MaxFilenameLength>("\\");
#   else
    return igenericnamespace::tostring<ipath::MaxFilenameLength>("/");
#   endif
}

ipath& ipath::operator+=(const ipath& str2)
{
    for (size_t i = 0; i < str2.size(); ++i)
        push_back(str2[i]);
    return *this;
}

ipath operator+(const ipath& path1, const ipath& path2)
{
    ipath result = path1;
    result += path2;
    return result;
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

minitl::format<ifilename::MaxFilenameLength> ifilename::str() const
{
#   ifdef BE_PLATFORM_WIN32
    return igenericnamespace::tostring<ifilename::MaxFilenameLength>("\\");
#   else
    return igenericnamespace::tostring<ifilename::MaxFilenameLength>("/");
#   endif
}

ifilename operator+(const ipath& path, const ifilename& file)
{
    ifilename result("");
    for (size_t i = 0; i < path.size(); ++i)
        result.push_back(path[i]);
    for (size_t i = 0; i < file.size(); ++i)
        result.push_back(file[i]);
    return result;
}

}
