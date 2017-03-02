/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>
#include    <rtti/value.hh>
#include    <parsecontext.hh>


namespace BugEngine { namespace RTTI
{

be_api(RTTI)
Value parseValue(minitl::Allocator& arena,
                 Parser::DbContext& context,
                 const char* strBegin,
                 const char *strEnd,
                 u32 initialLine,
                 u32 initialColumn)
{
    ref<Parser::Node> result = buildParseTree(arena, context.errors, strBegin, strEnd,
                                              initialLine, initialColumn);
    if (result && result->resolve(context))
    {
        return Value(); //result->eval(context);
    }
    else
    {
        return Value();
    }
}

be_api(RTTI)
Value parseValue(minitl::Allocator& arena,
                 Parser::DbContext& context,
                 const Type& expectedType,
                 const char* strBegin,
                 const char *strEnd,
                 u32 initialLine,
                 u32 initialColumn)
{
    ref<Parser::Node> result = buildParseTree(arena, context.errors, strBegin, strEnd,
                                              initialLine, initialColumn);
    if (result && result->resolve(context))
    {
        if (result->isCompatible(expectedType))
        {
            return Value(); //result->eval(context);
        }
        else
        {
            context.error(Parser::ParseLocation(),
                          Parser::ErrorType("object can't be casted to %s") | expectedType);
            return Value();
        }
    }
    else
    {
        return Value();
    }
}

ref<Parser::Node> buildParseTree(minitl::Allocator& arena,
                                 Parser::ErrorList& errors,
                                 const char* strBegin,
                                 const char *strEnd,
                                 u32 initialLine,
                                 u32 initialColumn)
{
    Parser::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                                 errors, initialLine, initialColumn);
    int result = be_value_parse(&context);
    if (result != 0 || !errors.empty())
    {
        return ref<Parser::Node>();
    }
    else
    {
        return context.result;
    }
}

}}
