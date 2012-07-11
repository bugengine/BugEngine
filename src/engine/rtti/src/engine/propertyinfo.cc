/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

Value Property::get(Value& from) const
{
    be_assert(from.type().metaclass->isA(owner.metaclass), "getting property on object of type %s, while expecting type %s" | from.type().name() | owner.name());
    i32 offset = from.type().metaclass->offset - owner.metaclass->offset;
    return (*getter)((void*)((char*)from.rawget() + offset), from.isConst());
}

void Property::set(Value& from, const Value& value) const
{
    get(from) = value;
}

Value Property::getTag(const Type& type) const
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

Value Property::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

}}
