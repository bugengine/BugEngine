/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/taginfo.script.hh>
#include    <rtti/engine/call.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

const Value Method::Parameter::s_noDefaultValue;

Value Method::Parameter::getTag(const Type& tagType) const
{
    for (const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
    {
        if (tagType <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
    }
    return Value();
}

Value Method::Parameter::getTag(raw<const Class> tagType) const
{
    return getTag(Type::makeType(tagType, Type::Value, Type::Const, Type::Const));
}

Value Method::Overload::getTag(const Type& type) const
{
    for (const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
    {
        if (type <= tag->tag.type())
            return Value(Value::ByRef(tag->tag));
    }
    return Value();
}

Value Method::Overload::getTag(raw<const Class> type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

Value Method::doCall(Value* params, u32 nparams) const
{
    ArgInfo<Type>* args = static_cast<ArgInfo<Type>*>(malloca(sizeof(ArgInfo<Type>) * nparams));
    for (u32 i = 0; i < nparams; ++i)
    {
        new(&args[i]) ArgInfo<Type>(params[i].type());
    }
    raw<const Method> thisPtr = { this };
    CallInfo c = resolve(thisPtr, args, nparams);
    if (c.conversion < ConversionCost::s_incompatible)
    {
        return c.overload->call(params, nparams);
    }
    else
    {
        be_error("No overload can convert all parameters");
        return Value();
    }
}

}}
