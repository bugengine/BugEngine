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
private:
    const StringCache* init(const char *str);
    const StringCache* init(const char *begin, const char *end);
public:
    istring() NOTHROW;
    istring(const char *str) NOTHROW;
    istring(const char *begin, const char *end) NOTHROW;
    istring(const std::string& other);
    istring(const istring& other) NOTHROW;
    template< size_t SIZE >
    istring(const minitl::format<SIZE>& f)
        :   m_index(init(f))
    {
    }

    ~istring();

    istring& operator=(const istring&) NOTHROW;

    const char *c_str() const NOTHROW;
    std::string str()   const;
    size_t size() const NOTHROW;
    size_t hash() const NOTHROW;

    bool operator==(const istring& other) const NOTHROW;
    bool operator!=(const istring& other) const NOTHROW;
    bool operator<(const istring& other) const NOTHROW;
};

class be_api(CORE) igenericnamespace
{
    enum
    {
        MaxNamespaceSize = 32
    };
private:
    istring m_namespace[MaxNamespaceSize];
    size_t  m_size;
protected:
    igenericnamespace();
    explicit igenericnamespace(const istring& onlycomponent);
    igenericnamespace(const char *str, const char* sep);
    igenericnamespace(const char *begin, const char *end, const char* sep);
public:
    size_t size() const;
    const istring& operator[](size_t index) const;

    void push_back(const istring& component);
    void pop_back();
    void pop_front();

    std::string tostring(const std::string& separator) const;

    bool operator==(const igenericnamespace& other) const;
    bool operator!=(const igenericnamespace& other) const;
};

be_api(CORE) bool startswith(const igenericnamespace& start, const igenericnamespace& full);
be_api(CORE) bool operator<(const igenericnamespace& ns1, const igenericnamespace& ns2);
    
class be_api(CORE) inamespace : public igenericnamespace
{
public:
    explicit inamespace(const istring& onlycomponent);
    inamespace(const char *str);
    inamespace(const std::string& str);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~inamespace() {}

    std::string str() const;

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
    explicit ifilename(const istring& onlycomponent);
    ifilename(const char *str);
    ifilename(const std::string& str);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~ifilename() {}

    std::string str() const;
private:
    ifilename();
};

class be_api(CORE) ipath : public igenericnamespace
{
public:
    explicit ipath(const istring& onlycomponent);
    ipath(const char *str);
    ipath(const char *begin, const char *end);
    ipath(const std::string& str);
    //lint -e{1509} : no virtual table needed in the namespaces/paths, no pointer will be handled
    ~ipath() {}

    std::string str() const;

    ipath& operator+=(const ipath& other);
private:
    ipath();
};

be_api(CORE) ipath     operator+(const ipath& path1, const ipath& path2);
be_api(CORE) ifilename operator+(const ipath& path, const ifilename& file);

}

namespace minitl
{
inline size_t hash_value(const BugEngine::istring& key) { return key.hash(); }
}

/*****************************************************************************/
#endif
