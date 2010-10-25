/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_SCRIPT_HH_
#define BE_RTTI_TYPEINFO_SCRIPT_HH_
/*****************************************************************************/
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine
{

struct TypeInfo
{
    enum Type
    {
        Class,
        RefPtr,
        ConstRefPtr,
        WeakPtr,
        ConstWeakPtr,
        RawPtr,
        ConstRawPtr
    };
    enum Constness
    {
        Mutable,
        Const
    };
    raw<const RTTI::ClassInfo>      metaclass;
    Type                            type;
    Constness                       constness;

    size_t                          size() const;
    minitl::format<>                name() const;
    void                            copy(const void* source, void* dest) const;
    void                            create(void* obj) const;
    void                            destroy(void* obj) const;
    const RTTI::PropertyInfo*       property(const char *prop) const;
};

}

/*****************************************************************************/
#endif
