/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/value.inl>


namespace BugEngine { namespace RTTI
{

void ClassInfo::copy(const void* src, void* dst) const
{
    be_assert_recover(copyconstructor, "no copy for type %s" | name, return);
    (*copyconstructor)(src, dst);
}

void ClassInfo::destroy(void* src) const
{
    be_assert_recover(destructor, "no destructor for type %s" | name, return);
    (*destructor)(src);
}

Value ClassInfo::get(Value& from, istring propname) const
{
    static const ClassInfo* const s_metaClassInfo = be_typeid<ClassInfo>::klass();
    if (from.type().metaclass == s_metaClassInfo)
    {
        const ObjectInfo* o = from.as<const ClassInfo*>()->objects;
        while(o)
        {
            if (o->name == propname)
            {
                return o->value;
            }
            o = o->next;
        }
    }

    {
        const MethodInfo* m = methods;
        while(m)
        {
            if (m->name == propname)
            {
                return Value(m);
            }
            m = m->next;
        }
    }

    {
        const PropertyInfo* p = properties;
        while(p)
        {
            if (p->name == propname)
            {
                return p->get(from);
            }
            p = p->next;
        }
    }

    return Value();
}

bool ClassInfo::isA(const ClassInfo* klass) const
{
    const ClassInfo* ci = this;
    while (ci)
    {
        if (ci == klass)
            return true;
        ci = ci->parent;
    }
    return false;
}

Value ClassInfo::operator()(Value* params, u32 nparams) const
{
    return (*constructor)(params, nparams);
}

Value ClassInfo::getTag(const TypeInfo& type) const
{
    TagInfo* tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value ClassInfo::getTag(const ClassInfo* type) const
{
    return getTag(TypeInfo::makeType(type, TypeInfo::Class, TypeInfo::Mutable));
}

u32 ClassInfo::distance(const ClassInfo* other) const
{
    const ClassInfo* ci = this;
    u32 result = 0;
    while (ci)
    {
        if (ci == other)
            return result;
        ci = ci->parent;
        result++;
    }
    return TypeInfo::MaxTypeDistance;
}

int ClassInfo::test() const
{
    //be_info("Done some testing");
    return 1;
}

}

RTTI::ClassInfo* be_Namespace()
{
    static RTTI::ClassInfo ci = { "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };
    return &ci;
}

}
