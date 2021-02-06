/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STRING_ISTRING_HH_
#define BE_CORE_STRING_ISTRING_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>

namespace BugEngine {

class be_api(CORE) istring
{
private:
    u32 m_index;

private:
    u32 init(const char* str);
    u32 init(const char* begin, const char* end);

public:
    istring();
    istring(const char* str);
    istring(const char* begin, const char* end);
    istring(const istring& other);

    ~istring();

    istring& operator=(const istring&);

    const char* c_str() const;
    size_t      size() const;
    u32         hash() const;

    bool operator==(const istring& other) const;
    bool operator!=(const istring& other) const;
    bool operator<(const istring& other) const;
};

class be_api(CORE) igenericnamespace
{
private:
    enum Constants
    {
        MaxNamespaceSize = 8
    };

private:
    istring* m_namespace;
    char     m_buffer[sizeof(istring) * MaxNamespaceSize];
    u16      m_size;
    u16      m_capacity;

private:
    void grow(u16 newCapacity);

protected:
    igenericnamespace();
    explicit igenericnamespace(const istring& onlycomponent);
    igenericnamespace(const igenericnamespace& other);
    igenericnamespace(const char* str, const char* sep);
    igenericnamespace(const char* begin, const char* end, const char* sep);
    ~igenericnamespace();
    igenericnamespace& operator=(const igenericnamespace& other);
    void               str(char* buffer, char separator, u32 size) const;

public:
    u32            size() const;
    const istring& operator[](u32 index) const;

    void    push_back(const istring& component);
    istring pop_back();
    istring pop_front();

    bool operator==(const igenericnamespace& other) const;
    bool operator!=(const igenericnamespace& other) const;
};

be_api(CORE) bool startswith(const igenericnamespace& start, const igenericnamespace& full);
be_api(CORE) bool operator<(const igenericnamespace& ns1, const igenericnamespace& ns2);

class be_api(CORE) inamespace : public igenericnamespace
{
public:
    enum
    {
        Separator     = '.',
        MaxPathLength = 1024
    };
    struct Path
    {
        char name[MaxPathLength];
             operator const char*() const
        {
            return name;
        }
    };
    inamespace();
    explicit inamespace(const istring& onlycomponent);
    inamespace(const char* str);
    ~inamespace()
    {
    }
    inamespace& operator+=(const inamespace& other);
    inamespace& operator+=(const istring& component);

    Path str(char separator = Separator) const;
};

be_api(CORE) inamespace operator+(const istring& str1, const istring& str2);
be_api(CORE) inamespace operator+(const istring& str1, const inamespace& ns2);
be_api(CORE) inamespace operator+(const inamespace& ns1, const istring& str2);
be_api(CORE) inamespace operator+(const inamespace& ns1, const inamespace& ns2);

class ipath;

class be_api(CORE) ifilename : public igenericnamespace
{
public:
    enum
    {
        Separator         = '/',
        MaxFilenameLength = 1024
    };
    struct Filename
    {
        char name[MaxFilenameLength];
             operator const char*() const
        {
            return name;
        }
    };
    ifilename();
    explicit ifilename(const istring& onlycomponent);
    explicit ifilename(const char* str);
    explicit ifilename(const ipath& path);
    ~ifilename()
    {
    }

    Filename str(char separator = Separator) const;
};

class be_api(CORE) ipath : public igenericnamespace
{
public:
    enum
    {
        Separator         = '/',
        MaxFilenameLength = 1024
    };
    struct Filename
    {
        char name[MaxFilenameLength];
             operator const char*() const
        {
            return name;
        }
    };
    ipath();
    explicit ipath(const istring& onlycomponent);
    explicit ipath(const char* str);
    ipath(const char* begin, const char* end);
    ~ipath()
    {
    }

    ipath&   operator+=(const ipath& other);
    Filename str(char separator = Separator) const;
};

be_api(CORE) ipath     operator+(const ipath& path1, const ipath& path2);
be_api(CORE) ifilename operator+(const ipath& path, const ifilename& file);

}  // namespace BugEngine

#include <bugengine/minitl/format.hh>
#include <bugengine/minitl/hash.hh>

namespace minitl {

template <>
struct hash< BugEngine::istring >
{
    inline u32 operator()(const BugEngine::istring& v)
    {
        return v.hash();
    }
    inline int operator()(const BugEngine::istring& v1, const BugEngine::istring& v2)
    {
        return v1 == v2;
    }
};

template < u16 SIZE >
static inline const format< SIZE >& operator|(const format< SIZE >&     f,
                                              const BugEngine::istring& value)
{
    return f | value.c_str();
}

template < u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& f, const BugEngine::inamespace& value)
{
    BugEngine::inamespace::Path p = value.str();
    return f | p.name;
}

template < u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& f, const BugEngine::ipath& value)
{
    BugEngine::ipath::Filename p = value.str();
    return f | p.name;
}

template < u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& f, const BugEngine::ifilename& value)
{
    BugEngine::ifilename::Filename p = value.str();
    return f | p.name;
}

}  // namespace minitl

/**************************************************************************************************/
#endif
