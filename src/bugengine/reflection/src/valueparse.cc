/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/reflection/stdafx.h>
#include <bugengine/meta/value.hh>
#include <bugengine/reflection/valueparse.hh>
#include <parsecontext.hh>

namespace BugEngine { namespace Meta {

ref< AST::Node > parseValue(minitl::Allocator& arena, AST::MessageList& errors,
                            const char* strBegin, const char* strEnd, u32 initialLine,
                            u32 initialColumn)
{
    Parse::ParseContext context(arena, strBegin, strEnd ? strEnd : (strBegin + strlen(strBegin)),
                                errors, initialLine, initialColumn);
    int                 result = be_value_parse(&context);
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

}}  // namespace BugEngine::Meta
