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
class PropertyInfo;
}

struct be_api(RTTI) TypeInfo
{
    friend class Value;

    enum Type
    {
        TypeMask        = 0xF,
        ConstBit        = 0x10,

        Class           = 0,
        ConstRefPtr     = 1,
        ConstWeakPtr    = 2,
        ConstRawPtr     = 3,
        RefPtr          = ConstRefPtr | ConstBit,
        WeakPtr         = ConstWeakPtr | ConstBit,
        RawPtr          = ConstRawPtr | ConstBit,
    };
    enum Constness
    {
        Mutable = 0,
        Const = 1
    };

    ref<const RTTI::ClassInfo> const    metaclass;
    Type const                          type;
    Constness const                     constness;


    inline TypeInfo(ref<const RTTI::ClassInfo> metaclass, Type type, Constness constness);
    inline ~TypeInfo();


    u32                             size() const;
    minitl::format<>                name() const;
private:
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

TypeInfo::~TypeInfo()
{
}

}

/*****************************************************************************/
#endif

