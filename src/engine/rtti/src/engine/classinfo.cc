/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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

const istring Class::nameConstructor()
{
    static const istring result = "?new";
    return result;
}

const istring Class::nameDestructor()
{
    static const istring result = "?del";
    return result;
}

const istring Class::nameOperatorCall()
{
    static const istring result = "?()";
    return result;
}

const istring Class::nameOperatorIndex()
{
    static const istring result = "?[]";
    return result;
}

const istring Class::nameOperatorLessThan()
{
    static const istring result = "?<";
    return result;
}

const istring Class::nameOperatorGreaterThan()
{
    static const istring result = "?>";
    return result;
}

const istring Class::nameOperatorLessThanOrEqual()
{
    static const istring result = "?<=";
    return result;
}

const istring Class::nameOperatorGreaterThanOrEqual()
{
    static const istring result = "?>=";
    return result;
}

const istring Class::nameOperatorMultiply()
{
    static const istring result = "?*";
    return result;
}

const istring Class::nameOperatorDivide()
{
    static const istring result = "?*";
    return result;
}

const istring Class::nameOperatorModulo()
{
    static const istring result = "?%";
    return result;
}

const istring Class::nameOperatorAdd()
{
    static const istring result = "?+";
    return result;
}

const istring Class::nameOperatorSubstract()
{
    static const istring result = "?-";
    return result;
}

const istring Class::nameOperatorShiftLeft()
{
    static const istring result = "?<<";
    return result;
}

const istring Class::nameOperatorShiftRight()
{
    static const istring result = "?>>";
    return result;
}

const istring Class::nameOperatorBitwiseAnd()
{
    static const istring result = "?&";
    return result;
}

const istring Class::nameOperatorBitwiseOr()
{
    static const istring result = "?|";
    return result;
}

const istring Class::nameOperatorBitwiseXor()
{
    static const istring result = "?^";
    return result;
}

const istring Class::nameOperatorBitwiseNot()
{
    static const istring result = "?~";
    return result;
}

const istring Class::nameOperatorLogicalAnd()
{
    static const istring result = "?&&";
    return result;
}

const istring Class::nameOperatorLogicalOr()
{
    static const istring result = "?||";
    return result;
}

const istring Class::nameOperatorLogicalNot()
{
    static const istring result = "?!";
    return result;
}

const istring Class::nameOperatorEqual()
{
    static const istring result = "?==";
    return result;
}

const istring Class::nameOperatorNotEqual()
{
    static const istring result = "?!=";
    return result;
}

const istring Class::nameOperatorAssign()
{
    static const istring result = "?=";
    return result;
}

const istring Class::nameOperatorMultiplyAssign()
{
    static const istring result = "?*=";
    return result;
}

const istring Class::nameOperatorDivideAssign()
{
    static const istring result = "?/=";
    return result;
}

const istring Class::nameOperatorModuloAssign()
{
    static const istring result = "?%=";
    return result;
}

const istring Class::nameOperatorAddAssign()
{
    static const istring result = "?+=";
    return result;
}

const istring Class::nameOperatorSubstractAssign()
{
    static const istring result = "?-=";
    return result;
}

const istring Class::nameOperatorShiftLeftAssign()
{
    static const istring result = "?<<=";
    return result;
}

const istring Class::nameOperatorShiftRightAssign()
{
    static const istring result = "?>>=";
    return result;
}

const istring Class::nameOperatorAndAssign()
{
    static const istring result = "?&=";
    return result;
}

const istring Class::nameOperatorOrAssign()
{
    static const istring result = "?|=";
    return result;
}

const istring Class::nameOperatorXorAssign()
{
    static const istring result = "?^=";
    return result;
}

const istring Class::nameOperatorIncrement()
{
    static const istring result = "?++";
    return result;
}

const istring Class::nameOperatorDecrement()
{
    static const istring result = "?--";
    return result;
}

const istring Class::nameOperatorGet()
{
    static const istring result = "?->";
    return result;
}

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

raw<const Property> Class::getProperty(istring propertyName) const
{
    raw<const Class> thisCls = { this };
    for (raw< const Class > cls = thisCls; cls; cls = cls->parent)
    {
        for (const Property* p = cls->properties->begin();
             p != cls->properties->end();
             ++p)
        {
            if (p->name == propertyName)
            {
                raw<const Property> pptr = {p};
                return pptr;
            }
        }
    }
    return raw<const Property>();
}

raw<const Method> Class::getMethod(istring methodName) const
{
    raw<const Class> thisCls = { this };
    for (raw< const Class > cls = thisCls; cls; cls = cls->parent)
    {
        for (const Method* m = cls->methods->begin();
             m != cls->methods->end();
             ++m)
        {
            if (m->name == methodName)
            {
                raw< const Method > mptr = { m };
                return mptr;
            }
        }
    }
    return raw<const Method>();
}

raw<const ObjectInfo> Class::getStaticProperty(istring propertyName) const
{
    raw<const ObjectInfo> o = objects;
    while(o)
    {
        if (o->name == propertyName)
        {
            break;
        }
        o = o->next;
    }
    return o;
}

Value Class::get(Value& from, istring propname, bool& found) const
{
    static raw<const Class> const s_metaClass = be_typeid<Class>::klass();
    if (from.type().metaclass == s_metaClass)
    {
        raw<const Class> cls = from.as< raw<const Class> >();
        raw<const ObjectInfo> o = cls->getStaticProperty(propname);
        if (o)
        {
            found = true;
            return o->value;
        }
        raw<const Method> m = cls->getMethod(propname);
        if (m)
        {
            found = true;
            return Value(m);
        }
    }

    raw<const Property> p = getProperty(propname);
    if (p)
    {
        found = true;
        return p->get(from);
    }
    raw<const Method> m = getMethod(propname);
    if (m)
    {
        found = true;
        return Value(m);
    }

    found = false;
    return Value();
}

Value Class::get(const Value& from, istring propname, bool& found) const
{
    static raw<const Class> const s_metaClass = be_typeid<Class>::klass();
    if (from.type().metaclass == s_metaClass)
    {
        raw<const Class> cls = from.as< raw<const Class> >();
        raw<const ObjectInfo> o = cls->getStaticProperty(propname);
        if (o)
        {
            found = true;
            return o->value;
        }
        raw<const Method> m = cls->getMethod(propname);
        if (m)
        {
            found = true;
            return Value(m);
        }
    }

    raw<const Property> p = getProperty(propname);
    if (p)
    {
        found = true;
        return p->get(from);
    }
    raw<const Method> m = getMethod(propname);
    if (m)
    {
        found = true;
        return Value(m);
    }

    found = false;
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
    raw<const Class> thisCls = { this };
    for (raw< const Class > cls = thisCls; cls; cls = cls->parent)
    {
        if (cls->tags)
        {
            for (const Tag* tag = cls->tags->begin();
                 tag != cls->tags->end();
                 ++tag)
            {
                if (type <= tag->tag.type())
                    return Value(Value::ByRef(tag->tag));
            }
        }
    }
    return Value();
}

Value Class::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

bool Class::distance(raw<const Class> other, u16& result) const
{
    raw<const Class> ci = {this};
    result = 0;
    while (ci)
    {
        if (ci == other)
            return true;
        ci = ci->parent;
        ++result;
    }
    return false;
}

inamespace Class::fullname() const
{
    if (!owner)
    {
        return inamespace(name);
    }
    else
    {
        return owner->fullname() + name;
    }
}

Value Class::findClass(inamespace name)
{
    Value v = Value(raw<const RTTI::Class>(be_game_Namespace()));
    while (v && name.size())
    {
        v = v[name.pop_front()];
    }
    return v;
}

}

raw<RTTI::Class> be_game_Namespace()
{
    static RTTI::Class ci = { "BugEngine", 0, 0, RTTI::ClassType_Namespace, {0}, {0}, {0},
                              {RTTI::staticarray<const RTTI::Tag>::s_null},
                              {RTTI::staticarray<const RTTI::Property>::s_null},
                              {RTTI::staticarray<const RTTI::Method>::s_null},
                              {0}, 0, 0 };
    raw<RTTI::Class> result = {&ci};
    return result;
}

raw<RTTI::Class> be_game_Namespace_BugEngine()
{
    return be_game_Namespace();
}

static RTTI::ObjectInfo ob = { be_game_Namespace_BugEngine()->objects, {0}, "BugEngine", RTTI::Value(be_game_Namespace()) };
BE_EXPORT const RTTI::ObjectInfo* obptr = (be_game_Namespace_BugEngine()->objects.m_ptr = &ob);

}
