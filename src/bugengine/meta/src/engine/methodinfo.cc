/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/call.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/taginfo.script.hh>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

const Value Method::Parameter::s_noDefaultValue;

Value Method::Parameter::getTag(const Type& tagType) const
{
    if(tags)
    {
        for(const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
        {
            if(tagType <= tag->tag.type()) return Value(Value::ByRef(tag->tag));
        }
    }
    return Value();
}

Value Method::Parameter::getTag(raw< const Class > tagType) const
{
    return getTag(Type::makeType(tagType, Type::Value, Type::Const, Type::Const));
}

Value Method::Overload::getTag(const Type& type) const
{
    if(tags)
    {
        for(const Tag* tag = tags->begin(); tag != tags->end(); ++tag)
        {
            if(type <= tag->tag.type()) return Value(Value::ByRef(tag->tag));
        }
    }
    return Value();
}

Value Method::Overload::getTag(raw< const Class > type) const
{
    return getTag(Type::makeType(type, Type::Value, Type::Const, Type::Const));
}

minitl::format< 1024u > Method::Overload::signature() const
{
    char  buffer[1023];
    char* current = buffer;
    char* end     = buffer + 1022;
    *end          = 0;
    for(u32 i = 0; i < params.count; ++i)
    {
        minitl::format< 1024u > argType = params.elements[i].type.name();
        for(const char* arg = argType; *arg && current != end; ++arg, ++current)
            *current = *arg;
        if(current != end) *(current++) = ' ';
        for(const char* arg = params.elements[i].name.c_str(); *arg && current != end;
            ++arg, ++current)
            *current = *arg;
        if(i < params.count - 1)
        {
            if(current != end) *(current++) = ',';
            if(current != end) *(current++) = ' ';
        }
    }
    *current = 0;
    if(current == end)
    {
        *(current - 3) = '.';
        *(current - 2) = '.';
        *(current - 1) = '.';
    }
    return minitl::format< 1024u >(buffer);
}

Value Method::doCall(Value* params, u32 nparams) const
{
    ArgInfo< Type >* args
        = static_cast< ArgInfo< Type >* >(malloca(sizeof(ArgInfo< Type >) * nparams));
    for(u32 i = 0; i < nparams; ++i)
    {
        new(&args[i]) ArgInfo< Type >(params[i].type());
    }
    raw< const Method > thisPtr = {this};
    CallInfo            c       = resolve(thisPtr, args, nparams);
    if(c.conversion < ConversionCost::s_incompatible)
    {
        if(c.overload->extraParameters.count == 0)
            return c.overload->call(params, nparams);
        else
        {
            Value* values = static_cast< Value* >(
                malloca(sizeof(Value) * (nparams + c.overload->extraParameters.count)));
            for(u32 i = 0; i < c.overload->extraParameters.count; ++i)
            {
                new(values + i) Value(c.overload->extraParameters[i]);
            }
            for(u32 i = 0; i < nparams; ++i)
            {
                new(values + c.overload->extraParameters.count + i) Value(params[i]);
            }
            Value v = c.overload->call(values, c.overload->extraParameters.count + nparams);
            for(u32 i = c.overload->extraParameters.count + nparams; i > 0; --i)
            {
                values[i - 1].~Value();
            }
            freea(values);
            return v;
        }
    }
    else
    {
        be_error("No overload can convert all parameters");
        return Value();
    }
}

}}  // namespace BugEngine::Meta
