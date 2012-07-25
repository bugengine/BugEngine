/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STRING_ISTRING_HH_
#define BE_CORE_STRING_ISTRING_HH_
/*****************************************************************************/

namespace BugEngine
{

class StringCache;

class be_api(CORE) istring
{
private:
    const StringCache* m_index;
private:
    const StringCache* init(const char *str);
    const StringCache* init(const char *begin, const char *end);
public:
    istring();
    istring(const char *str);
    istring(const char *begin, const char *end);
    istring(const istring& other);

    ~istring();

    istring& operator=(const istring&);

    const char *c_str() const;
    size_t size() const;
    u64 hash() const;

    bool operator==(const istring& other) const;
    bool operator!=(const istring& other) const;
    bool operator<(const istring& other) const;
};

class be_api(CORE) igenericnamespace
{
public:
    enum Constants { MaxNamespaceSize = 15 };
private:
    istring m_namespace[MaxNamespaceSize];
    u32     m_size;
protected:
    igenericnamespace();
    explicit igenericnamespace(const istring& onlycomponent);
    igenericnamespace(const char *str, const char* sep);
    igenericnamespace(const char *begin, const char *end, const char* sep);
    ~igenericnamespace() {}
    void str(char* buffer, char separator, u32 size) const;
public:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
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
    enum { Separator = '.', MaxPathLength = 1024 };
    struct Path
    {
        char name[MaxPathLength];
    };
    explicit inamespace(const istring& onlycomponent);
    inamespace(const char *str);
    ~inamespace() {}
    inamespace& operator+=(const inamespace& other);
    inamespace& operator+=(const istring& component);

    Path str() const;
private:
    inamespace();
};

be_api(CORE) inamespace operator+(const istring& str1, const istring& str2);
be_api(CORE) inamespace operator+(const istring& str1, const inamespace& ns2);
be_api(CORE) inamespace operator+(const inamespace& ns1, const istring& str2);
be_api(CORE) inamespace operator+(const inamespace& ns1, const inamespace& ns2);

class be_api(CORE) ifilename : public igenericnamespace
{
public:
    enum { Separator = '/', MaxFilenameLength = 1024 };
    struct Filename
    {
        char name[MaxFilenameLength];
    };
    explicit ifilename(const istring& onlycomponent);
    ifilename(const char *str);
    ~ifilename() {}

    Filename str() const;
private:
    ifilename();
};

class be_api(CORE) ipath : public igenericnamespace
{
public:
    enum { Separator = '/', MaxFilenameLength = 1024 };
    struct Filename
    {
        char name[MaxFilenameLength];
    };
    explicit ipath(const istring& onlycomponent);
    ipath(const char *str);
    ipath(const char *begin, const char *end);
    ~ipath() {}

    ipath& operator+=(const ipath& other);
    Filename str() const;
private:
    ipath();
};

be_api(CORE) ipath     operator+(const ipath& path1, const ipath& path2);
be_api(CORE) ifilename operator+(const ipath& path, const ifilename& file);

}

#include    <minitl/format.hh>
#include    <minitl/hash_map.hh>
namespace minitl
{

template<>
struct hash<BugEngine::istring>
{
    inline u64 operator()(const BugEngine::istring& v)                               { return v.hash(); }
    inline int operator()(const BugEngine::istring& v1, const BugEngine::istring& v2){ return v1 == v2; }
};

inline size_t hash_value(const BugEngine::istring& key) { return (size_t)key.hash(); }

template< u16 SIZE >
static inline const format<SIZE>& operator|(const format<SIZE>& f, const BugEngine::istring& value)
{
    return f | value.c_str();
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const BugEngine::inamespace& value)
{
    if (value.size() > 0)
    {
        f | value[0];
        for (u32 i = 1; i < value.size(); ++i)
        {
            f | (char)value.Separator;
            f | value[i];
        }
    }
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const BugEngine::ipath& value)
{
    if (value.size() > 0)
    {
        f | value[0];
        for (u32 i = 1; i < value.size(); ++i)
        {
            f | (char)value.Separator;
            f | value[i];
        }
    }
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const BugEngine::ifilename& value)
{
    if (value.size() > 0)
    {
        f | value[0];
        for (u32 i = 1; i < value.size(); ++i)
        {
            f | (char)value.Separator;
            f | value[i];
        }
    }
    return f;
}

}

/*****************************************************************************/
#endif
