/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_ISTRING_HH_
#define BE_CORE_ISTRING_HH_
/*****************************************************************************/

namespace BugEngine
{
# pragma warning(disable:4251)

class StringCache;

class be_api(CORE) istring
{
private:
    const StringCache* m_index;
#ifdef BE_DEBUG
    const char *m_str;
#endif
private:
    const StringCache* init(const char *str);
    const StringCache* init(const char *begin, const char *end);
public:
    istring();
    istring(const char *str);
    istring(const char *begin, const char *end);
    istring(const istring& other);
    template< u16 SIZE >
    istring(const minitl::format<SIZE>& f)
        :   m_index(init(f))
    {
    }

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
    template< u16 MAXLENGTH >
    minitl::format<MAXLENGTH> tostring(const char* sep) const
    {
        minitl::format<MAXLENGTH> result("");
        if (m_size > 0)
        {
            result.append(m_namespace[0].c_str());
            for (size_t i = 1; i < m_size; ++i)
            {
                result.append(sep);
                result.append(m_namespace[i].c_str());
            }
        }
        return result;
    }
public:
    size_t size() const;
    const istring& operator[](size_t index) const;

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
    enum { MaxNamespaceLength = 4096 };
    explicit inamespace(const istring& onlycomponent);
    inamespace(const char *str);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~inamespace() {}

    minitl::format<MaxNamespaceLength> str() const;

    inamespace& operator+=(const inamespace& other);
    inamespace& operator+=(const istring& component);
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
    enum { MaxFilenameLength = 1024 };
    explicit ifilename(const istring& onlycomponent);
    ifilename(const char *str);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~ifilename() {}

    minitl::format<MaxFilenameLength> str() const;
private:
    ifilename();
};

class be_api(CORE) ipath : public igenericnamespace
{
public:
    enum { MaxFilenameLength = 1024 };
    explicit ipath(const istring& onlycomponent);
    ipath(const char *str);
    ipath(const char *begin, const char *end);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~ipath() {}

    minitl::format<MaxFilenameLength> str() const;

    ipath& operator+=(const ipath& other);
private:
    ipath();
};

be_api(CORE) ipath     operator+(const ipath& path1, const ipath& path2);
be_api(CORE) ifilename operator+(const ipath& path, const ifilename& file);

}

#include <minitl/string/format.hh>

namespace minitl
{
inline size_t hash_value(const BugEngine::istring& key) { return (size_t)key.hash(); }

template< u16 size >
const format<size>& operator|(const format<size>& f, const BugEngine::istring& value)
{
    return f | value.c_str();
}

template< u16 size >
const format<size>& operator|(const format<size>& f, const BugEngine::inamespace& value)
{
    format<4096u> s = value.str();
    return f | s.c_str();
}

template< u16 size >
const format<size>& operator|(const format<size>& f, const BugEngine::ipath& value)
{
    format<1024u> s = value.str();
    return f | s.c_str();
}

template< u16 size >
const format<size>& operator|(const format<size>& f, const BugEngine::ifilename& value)
{
    format<1024u> s = value.str();
    return f | s.c_str();
}

template<>
struct hash<BugEngine::istring>
{
    inline u64 operator()(const BugEngine::istring& v)                               { return v.hash(); }
    inline int operator()(const BugEngine::istring& v1, const BugEngine::istring& v2){ return v1 == v2; }
};

}

/*****************************************************************************/
#endif
