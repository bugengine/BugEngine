/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_SCRIPT_HH_
#define BE_RTTI_TYPEINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine
{

class Value;

namespace RTTI
{
struct ClassInfo;
}

struct be_api(RTTI) TypeInfo
{
    friend class Value;

    enum Type
    {
        TypeMask        = 0xF,
        MutableBit      = 0x10,

        Class           = 0,
        ConstRawPtr     = 1,
        ConstWeakPtr    = 2,
        ConstRefPtr     = 3,
        RawPtr          = ConstRawPtr | MutableBit,
        WeakPtr         = ConstWeakPtr | MutableBit,
        RefPtr          = ConstRefPtr | MutableBit
    };
    enum Constness
    {
        Const = 0,
        Mutable = 1
    };
    enum ConstifyType
    {
        Constify
    };

    raw<const RTTI::ClassInfo>  metaclass;
    Type                        type;
    Constness                   constness;

    static inline TypeInfo makeType(raw<const RTTI::ClassInfo> klass, Type type, Constness constness = Mutable)
    {
        TypeInfo info = { klass, type, constness };
        return info;
    }
    static inline TypeInfo makeType(const TypeInfo& type, ConstifyType /*constify*/)
    {
        TypeInfo info = { type.metaclass, type.type, Const };
        return info;
    }

    u32                             size() const;
    minitl::format<>                name() const;
    u32                             distance(const TypeInfo& other) const;
public:
    enum { MaxTypeDistance = 1000000 };
private:
    TypeInfo& operator=(const TypeInfo&);
    void*                           rawget(const void*) const;
    void                            copy(const void* source, void* dest) const;
    void                            create(void* obj) const;
    void                            destroy(void* obj) const;
};

}

/*****************************************************************************/
#endif

