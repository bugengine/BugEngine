/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <parsecontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

ParseContext* g_parseContext;
static i_u32  s_useCount;

ParseContext::ParseContext(minitl::Allocator& allocator, const char* bufferStart,
                           const char* bufferEnd, MessageList& errors, u32 lineStart,
                           u32 columnStart)
    : arena(&allocator)
    , result()
    , bufferStart(bufferStart)
    , bufferEnd(bufferEnd)
    , buffer(bufferStart)
    , errors(errors)
    , location()
{
    if(s_useCount++ != 0)
    {
        be_error("RTTI Parser is not reentrant");
    }
    location.line        = lineStart;
    location.columnStart = columnStart;
    location.columnEnd   = columnStart;
    g_parseContext       = this;
}

ParseContext::~ParseContext()
{
    s_useCount--;
    g_parseContext = 0;
}

}}}  // namespace BugEngine::RTTI::AST
