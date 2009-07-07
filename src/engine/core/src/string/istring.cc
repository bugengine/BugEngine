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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/string/istring.hh>
#include    <core/threads/criticalsection.hh>
#include    <cstring>


namespace BugEngine
{


class StringCache
{
    friend class StringInit;
private:
    class Buffer
    {
    private:
        Buffer*     m_next;
        byte*       m_buffer;
        i_size_t    m_used;
        static const size_t s_capacity = 1024*200;
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
    typedef minitl::map< const char *, StringCache*, minitl::less<const char *> > StringIndex;
private:
    static Buffer*  getBuffer() NOTHROW;
public:
    static StringCache* unique(const char *val) NOTHROW;
    static StringCache* unique(const char *begin, const char *end) NOTHROW;
private:
    mutable i_u32  m_refCount;
    size_t  m_hash;
    size_t  m_length;
#   ifndef  NDEBUG
    size_t  m_gard;
#   endif
private:
    StringCache(size_t _hash, size_t len) NOTHROW :
        m_refCount(0),
        m_hash(_hash),
        m_length(len)
#   ifndef  NDEBUG
        ,m_gard(0xDEADBEEF)
#   endif
    {
    }
    StringCache();
public:
    void retain(void) const NOTHROW     { m_refCount++; }
    void release(void) const NOTHROW    { Assert(m_refCount); m_refCount--; }
    size_t hash() const NOTHROW         { return m_hash; }
    size_t size() const NOTHROW         { return m_length; }
    const char *str() const NOTHROW     { return reinterpret_cast<const char *>(this+1); }
};

StringCache::Buffer* StringCache::getBuffer() NOTHROW
{
    static Buffer s_root;
    return &s_root;
}

StringCache::Buffer::Buffer()
:   m_next(0)
,   m_buffer((byte*)be_malloc(s_capacity))
,   m_used(0)
{
}

StringCache::Buffer::~Buffer()
{
    delete m_next;
    be_free(m_buffer);
}

StringCache* StringCache::Buffer::reserve(size_t size)
{
    size_t allsize = BE_ALIGN(size+1+sizeof(StringCache), BE_ALIGNOF(StringCache));
    Assert(allsize < s_capacity);
    if(m_used > s_capacity)
    {
        return reserveNext(size);
    }
    size_t offset = m_used.addExchange(allsize);
    if(offset+allsize < s_capacity)
    {
        Assert(BE_ALIGN(offset, BE_ALIGNOF(StringCache)) == offset);
        return (StringCache*)(m_buffer+offset);
    }
    else
    {
        return reserveNext(size);
    }
}

StringCache* StringCache::Buffer::reserveNext(size_t size)
{
    if(!m_next)
    {
        m_next = new Buffer;
    }
    return m_next->reserve(size);
}

StringCache* StringCache::unique(const char *begin, const char *end) NOTHROW
{
    size_t _size = 1+size_t(end)-size_t(begin);
    char *val = static_cast<char*>(malloca(_size));
    strncpy(val,begin,_size-1);
    val[end-begin] = 0;
    StringCache* result = unique(val);
    freea(val);
    return result;
}

StringCache* StringCache::unique(const char *val) NOTHROW
{
    static CriticalSection s_lock;
    ScopedCriticalSection scope(s_lock);
    try
    {
        static StringCache::StringIndex g_strings;
        StringIndex::const_iterator it;
        it = g_strings.find(val);
        if(it != g_strings.end())
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

            std::pair<StringIndex::iterator,bool> insertresult = g_strings.insert(std::make_pair(data, cache));
            Assert(insertresult.second);
            return cache;
        }
    }
    catch(...)
    {
        AssertNotReached();
        exit(1);
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

istring::istring() NOTHROW :
    m_index(init(""))
{
}

istring::istring(const char *value) NOTHROW :
    m_index(init(value))
{
}

istring::istring(const char *begin, const char *end) NOTHROW :
    m_index(init(begin,end))
{
}

istring::istring(const std::string& other) :
    m_index(init(other.c_str()))
{
}

istring::istring(const istring& other) NOTHROW :
    m_index(other.m_index)
{
    m_index->retain();
}

istring::~istring()
{
    m_index->release();
}

istring& istring::operator=(const istring& other) NOTHROW
{
    if(&other == this)
        return *this;
    other.m_index->retain();
    m_index->release();
    m_index = other.m_index;
    return *this;
}

size_t istring::hash() const NOTHROW
{
    return m_index->hash();
}

size_t istring::size() const NOTHROW
{
    return m_index->size();
}

const char *istring::c_str() const NOTHROW
{
    return m_index->str();
}

std::string istring::str() const
{
    return std::string(c_str());
}

bool istring::operator==(const istring& other) const NOTHROW
{
    return other.m_index == m_index;
}

bool istring::operator!=(const istring& other) const NOTHROW
{
    return other.m_index != m_index;
}

bool istring::operator<(const istring& other) const NOTHROW
{
    return m_index < other.m_index;
}

//-----------------------------------------------------------------------------
static inline const char* findnext(const char* str, const char *sep, size_t len)
{
    const char *result = str;
    while(*result)
    {
        for(size_t i = 0; i < len && *result; ++i)
            if(*result == sep[i])
                return result;
        result++;
    }
    return result;
}

static void parse(const char *str, const char *end, const char *sep, istring* buffer, size_t& size)
{
    size_t numsep = strlen(sep);

    if(str && *str)
    {
        const char *ss = str;
        while(*ss && ss < end)
        {
            ss = findnext(str, sep, numsep);
            Assert(ss != str);    
            buffer[size++] = istring(str,ss);
            if(*ss) ss++;
            str=ss;
        }
    }
    return;
}

igenericnamespace::igenericnamespace() :
    m_namespace(),
    m_size(0)
{
}

igenericnamespace::igenericnamespace(const istring& onlycomponent) :
    m_namespace(),
    m_size(1)
{
    m_namespace[0] = onlycomponent;
}

igenericnamespace::igenericnamespace(const char *begin, const char *end, const char* sep) :
    m_namespace(),
    m_size(0)
{
    parse(begin, end, sep, m_namespace, m_size);
}

igenericnamespace::igenericnamespace(const char *str, const char* sep) :
    m_namespace(),
    m_size(0)
{
    parse(str, str+strlen(str), sep, m_namespace, m_size);
}

std::string igenericnamespace::tostring(const std::string& sep) const
{
    if(m_size == 0)
        return "";

    size_t len = m_size;
    for(size_t i = 1; i < m_size; ++i)
    {
        len += sep.size();
        len += m_namespace[i].size();
    }

    std::string result;
    result.reserve(len+1);
    result += m_namespace[0].c_str();
    for(size_t i = 1; i < m_size; ++i)
    {
        result += sep;
        result += m_namespace[i].c_str();
    }
    return result;
}

size_t igenericnamespace::size() const
{
    return m_size;
}

const istring& igenericnamespace::operator[](size_t index) const
{
    Assert(m_size > index);
    return m_namespace[index];
}

void igenericnamespace::push_back(const istring& component)
{
    Assert(m_size < MaxNamespaceSize);
    m_namespace[m_size++] = component;
}

void igenericnamespace::pop_back()
{
    m_size --;
}

void igenericnamespace::pop_front()
{
    for(size_t i = 1; i < m_size; ++i)
        m_namespace[i-1] = m_namespace[i];
    m_size--;
}

bool igenericnamespace::operator==(const igenericnamespace& other) const
{
    if( this->size() != other.size())
        return false;
    for(size_t i = 0; i < other.size(); ++i)
        if(m_namespace[i] != other[i])
            return false;
    return true;
}

bool igenericnamespace::operator!=(const igenericnamespace& other) const
{
    if( this->size() != other.size())
        return true;
    for(size_t i = 0; i < other.size(); ++i)
        if(m_namespace[i] != other[i])
            return true;
    return false;
}

bool startswith(const igenericnamespace& start, const igenericnamespace& full)
{
    for(size_t i = 0; i < start.size(); ++i)
        if(start[i] != full[i])
            return false;
    return true;
}

bool operator<(const igenericnamespace& ns1, const igenericnamespace& ns2)
{
    for(size_t i = 0; i < std::min(ns1.size(), ns2.size()); ++i)
    {
        if(ns1[i] < ns2[i])
            return true;
        else if(ns2[i] < ns1[i])
            return false;
    }
    return ns1.size() < ns2.size();
}

//-----------------------------------------------------------------------------

inamespace::inamespace(const istring& onlycomponent) :
    igenericnamespace(onlycomponent)
{
}

inamespace::inamespace(const char* _str) :
    igenericnamespace(_str, ".")
{
}

inamespace::inamespace(const std::string& _str) :
    igenericnamespace(_str.c_str(), ".")
{
}

std::string inamespace::str() const
{
    return igenericnamespace::tostring(".");
}

inamespace& inamespace::operator+=(const istring& str2)
{
    return this->operator +=(inamespace(str2));
}

inamespace& inamespace::operator+=(const inamespace& str2)
{
    for(size_t i = 0; i < str2.size(); ++i)
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

ipath::ipath(const istring& onlycomponent) :
    igenericnamespace(onlycomponent)
{
}

ipath::ipath(const char *_str) :
    igenericnamespace(_str, "/\\")
{
}

ipath::ipath(const char *begin, const char *end) :
    igenericnamespace(begin, end, "/\\")
{
}

ipath::ipath(const std::string& _str) :
    igenericnamespace(_str.c_str(), "/\\")
{
}

std::string ipath::str() const
{
#   ifdef WIN32
    return igenericnamespace::tostring("\\");
#   else
    return igenericnamespace::tostring("/");
#   endif
}

ipath& ipath::operator+=(const ipath& str2)
{
    for(size_t i = 0; i < str2.size(); ++i)
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
    
ifilename::ifilename(const istring& onlycomponent) :
    igenericnamespace(onlycomponent)
{
}

ifilename::ifilename(const char *_str) :
    igenericnamespace(_str, "/\\")
{
}

ifilename::ifilename(const std::string& _str) :
    igenericnamespace(_str.c_str(), "/\\")
{
}

std::string ifilename::str() const
{
#   ifdef WIN32
    return igenericnamespace::tostring("\\");
#   else
    return igenericnamespace::tostring("/");
#   endif
}

ifilename operator+(const ipath& path, const ifilename& file)
{
    ifilename result("");
    for(size_t i = 0; i < path.size(); ++i)
        result.push_back(path[i]);
    for(size_t i = 0; i < file.size(); ++i)
        result.push_back(file[i]);
    return result;
}

}
