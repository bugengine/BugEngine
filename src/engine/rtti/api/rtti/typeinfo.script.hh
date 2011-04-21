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
class ClassInfo;
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

    ref<const RTTI::ClassInfo> const    metaclass;
    Type const                          type;
    Constness const                     constness;

    inline TypeInfo(ref<const RTTI::ClassInfo> metaclass, Type type, Constness constness);
    inline TypeInfo(const TypeInfo& other, ConstifyType);
    inline ~TypeInfo();

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


TypeInfo::TypeInfo(ref<const RTTI::ClassInfo> metaclass, Type type, Constness constness)
    :   metaclass(metaclass)
    ,   type(type)
    ,   constness(constness)
{
}

TypeInfo::TypeInfo(const TypeInfo& other, ConstifyType)
    :   metaclass(other.metaclass)
    ,   type(other.type)
    ,   constness(Const)
{
}

TypeInfo::~TypeInfo()
{
}

}

/*****************************************************************************/
#endif

