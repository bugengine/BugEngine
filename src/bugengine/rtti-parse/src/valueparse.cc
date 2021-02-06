/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-parse/stdafx.h>
#include <bugengine/rtti-parse/valueparse.hh>
#include <bugengine/rtti/value.hh>
#include <parsecontext.hh>

namespace BugEngine { namespace RTTI {

ref< AST::Node > parseValue(minitl::Allocator& arena, AST::MessageList& errors,
                            const char* strBegin, const char* strEnd, u32 initialLine,
                            u32 initialColumn)
{
    AST::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                              errors, false, initialLine, initialColumn);
    int               result = be_value_parse(&context);
    be_value_lex_destroy();
    if(result != 0 || !errors.empty())
    {
        return ref< AST::Node >();
    }
    else
    {
        return context.result;
    }
}

ref< AST::Object > parseObject(minitl::Allocator& arena, AST::MessageList& errors,
                               const char* strBegin, const char* strEnd, u32 initialLine,
                               u32 initialColumn)
{
    AST::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                              errors, true, initialLine, initialColumn);
    int               result = be_value_parse(&context);
    be_value_lex_destroy();
    if(result != 0 || !errors.empty())
    {
        return ref< AST::Object >();
    }
    else
    {
        return be_checked_cast< AST::Object >(context.result);
    }
}

}}  // namespace BugEngine::RTTI
