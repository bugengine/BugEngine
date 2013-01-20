/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

void Class::copy(const void* src, void* dst) const
{
    be_assert_recover(copyconstructor, "no copy for type %s" | name, return);
    (*copyconstructor)(src, dst);
}

void Class::destroy(void* src) const
{
    be_assert_recover(destructor, "no destructor for type %s" | name, return);
    (*destructor)(src);
}

void Class::enumerateObjects(EnumerateRecursion recursion, EnumerateCallback callback) const
{
    static raw<const Class> const s_metaClass = be_typeid<Class>::klass();
    raw<const ObjectInfo> o = objects;
    while(o)
    {
        (*callback)(o->value);
        if (recursion == EnumerateRecursive && (o->value.type().metaclass == s_metaClass))
        {
            o->value.as< raw<const Class> >()->enumerateObjects(recursion, callback);
        }
        o = o->next;
    }
}

Value Class::get(Value& from, istring propname) const
{
    static raw<const Class> const s_metaClass = be_typeid<Class>::klass();
    if (from.type().metaclass == s_metaClass)
    {
        raw<const ObjectInfo> o = from.as< raw<const Class> >()->objects;
        while(o)
        {
            if (o->name == propname)
            {
                return o->value;
            }
            o = o->next;
        }
        raw<const Method> m = from.as< raw<const Class> >()->methods;
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
        raw<const Property> p = properties;
        while(p)
        {
            if (p->name == propname)
            {
                return p->get(from);
            }
            p = p->next;
        }
    }

    {
        raw<const Method> m = methods;
        while(m)
        {
            if (m->name == propname)
            {
                return Value(m);
            }
            m = m->next;
        }
    }

    be_error("unable to access member %s in class %s" | propname | from.type().metaclass->name);
    return Value();
}

bool Class::isA(raw<const Class> klass) const
{
    raw<const Class> ci = {this};
    while (ci)
    {
        if (ci == klass)
            return true;
        ci = ci->parent;
    }
    return false;
}

Value Class::getTag(const Type& type) const
{
    raw<Tag> tag = tags;
    while(tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
        tag = tag->next;
    }
    return Value();
}

Value Class::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

u32 Class::distance(raw<const Class> other) const
{
    raw<const Class> ci = {this};
    u32 result = 0;
    while (ci)
    {
        if (ci == other)
            return result;
        ci = ci->parent;
        result++;
    }
    return Type::MaxTypeDistance;
}

}

raw<RTTI::Class> be_game_Namespace()
{
    static RTTI::Class ci = { "BugEngine", {0}, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };
    raw<RTTI::Class> result = {&ci};
    return result;
}

raw<RTTI::Class> be_game_Namespace_BugEngine()
{
    return be_game_Namespace();
}

static RTTI::ObjectInfo ob = { be_game_Namespace_BugEngine()->objects, {0}, "BugEngine", RTTI::Value(be_game_Namespace()) };
static const RTTI::ObjectInfo* obptr = (be_game_Namespace_BugEngine()->objects.m_ptr = &ob);

}
