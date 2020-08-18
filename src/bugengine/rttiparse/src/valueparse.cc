/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/rtti/value.hh>
#include <bugengine/rttiparse/valueparse.hh>
#include <parsecontext.hh>

namespace BugEngine { namespace RTTI {

ref< Parser::Node > parseValue(minitl::Allocator& arena, Parser::MessageList& errors,
                               const char* strBegin, const char* strEnd, u32 initialLine,
                               u32 initialColumn)
{
    Parser::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                                 errors, false, initialLine, initialColumn);
    int                  result = be_value_parse(&context);
    if(result != 0 || !errors.empty())
    {
        return ref< Parser::Node >();
    }
    else
    {
        return context.result;
    }
}

ref< Parser::Object > parseObject(minitl::Allocator& arena, Parser::MessageList& errors,
                                  const char* strBegin, const char* strEnd, u32 initialLine,
                                  u32 initialColumn)
{
    Parser::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                                 errors, true, initialLine, initialColumn);
    int                  result = be_value_parse(&context);
    if(result != 0 || !errors.empty())
    {
        return ref< Parser::Object >();
    }
    else
    {
        return be_checked_cast< Parser::Object >(context.result);
    }
}

}}  // namespace BugEngine::RTTI
