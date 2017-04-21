/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <parsecontext.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

ParseContext* g_parseContext;
static i_u32 s_useCount;

ParseContext::ParseContext(minitl::Allocator& allocator,
                           const char* bufferStart, const char* bufferEnd,
                           ErrorList& errors,
                           bool expectObject,
                           u32 lineStart, u32 columnStart)
    :   arena(&allocator)
    ,   result()
    ,   bufferStart(bufferStart)
    ,   bufferEnd(bufferEnd)
    ,   buffer(bufferStart)
    ,   errors(errors)
    ,   location()
    ,   parseHeader(expectObject ? HeaderObject : HeaderAnyValue)
{
    if (s_useCount++ != 0)
    {
        be_error("RTTI Parser is not reentrant");
    }
    location.line = lineStart;
    location.columnStart = columnStart;
    location.columnEnd = columnStart;
    g_parseContext = this;
}

ParseContext::~ParseContext()
{
    s_useCount--;
    g_parseContext = 0;
}


}}}
