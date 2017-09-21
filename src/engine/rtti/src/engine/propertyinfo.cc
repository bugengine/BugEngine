/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

Value Property::get(Value& from) const
{
    be_assert(from.type().metaclass->isA(owner.metaclass), "getting property on object of type %s, while expecting type %s" | from.type() | owner);
    i32 offset = from.type().metaclass->offset - owner.metaclass->offset;
    return (*getter)((void*)((char*)from.rawget() + offset), from.isConst());
}

Value Property::get(const Value& from) const
{
    be_assert(from.type().metaclass->isA(owner.metaclass), "getting property on object of type %s, while expecting type %s" | from.type() | owner);
    i32 offset = from.type().metaclass->offset - owner.metaclass->offset;
    return (*getter)((void*)((char*)from.rawget() + offset), true);
}

void Property::set(Value& from, const Value& value) const
{
    get(from) = value;
}

Value Property::getTag(const Type& tagType) const
{
    if (tags)
    {
        for (const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
        {
            if (tagType <= tag->tag.type())
               return Value(Value::ByRef(tag->tag));
        }
    }
    return Value();
}

Value Property::getTag(raw<const Class> tagType) const
{
    return getTag(Type::makeType(tagType, Type::Value, Type::Const, Type::Const));
}

}}
