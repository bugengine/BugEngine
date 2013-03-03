/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_CLASSINFO_SCRIPT_HH_
/*****************************************************************************/
#include    <core/md5.hh>

namespace BugEngine { namespace RTTI
{

class Value;
struct ObjectInfo;
struct Property;
struct Method;
struct Tag;

enum ClassType
{
    ClassType_Object    = 0,
    ClassType_Struct    = 1,
    ClassType_Pod       = 2,
    ClassType_Namespace = 3,
    ClassType_Array     = 4,
    ClassType_Enum      = 5,
    ClassType_String    = 6,
    ClassType_Integer   = 7
};

struct be_api(RTTI) Class
{
    friend struct Type;
    friend class Value;
published:

    istring const               name;

    raw<const Class> const      owner;

    raw<const Class> const      parent;

    u32 const                   size;

    i32 const                   offset;

    u32 const                   id;

    raw<Tag>                    tags;

    raw<const Property>         properties;

    raw<const Method>           methods;

    raw<const ObjectInfo>       objects;

    be_tag(Alias("?call"))
    raw<const Method>           constructor;

    raw<const Method>           cast;
public:

    typedef void (*CopyConstructor)(const void* source, void* destination);
    typedef void (*Destructor)(void* object);
    const CopyConstructor   copyconstructor;
    const Destructor        destructor;
published:
    Value getTag(const Type& type) const;
    Value getTag(raw<const Class> type) const;

    Value get(Value& from, istring name) const;

    bool isA(raw<const Class> klass) const;

    inamespace fullname() const;

    inline ClassType type() const       { return ClassType(id & 0xffff); }
    inline u32 index() const            { return id >> 16; }

    raw<const Property> getProperty(istring name) const;
    raw<const Method> getMethod(istring name) const;
public:
    typedef void(*EnumerateCallback)(const Value& v);
    enum EnumerateRecursion
    {
        EnumerateOwn,
        EnumerateRecursive
    };
    void enumerateObjects(EnumerateRecursion recursion, EnumerateCallback callback) const;
    u32 distance(raw<const Class> other) const;
    const ObjectInfo* addObject(const istring& s, const ObjectInfo* ob);
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

be_api(RTTI) raw<RTTI::Class> be_game_Namespace();
raw<RTTI::Class> be_game_Namespace_BugEngine();

}

/*****************************************************************************/
#endif

