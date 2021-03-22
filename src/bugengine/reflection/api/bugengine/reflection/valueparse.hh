/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_PARSE_VALUEPARSE_HH_
#define BE_META_PARSE_VALUEPARSE_HH_
/**************************************************************************************************/
#include <bugengine/reflection/stdafx.h>
#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/introspect/node/node.hh>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta {

be_api(REFLECTION) ref< AST::Node > parseValue(minitl::Allocator& arena, AST::MessageList& context,
                                               const char* strBegin, const char* strEnd = 0,
                                               u32 initialLine = 0, u32 initialColumn = 0);

be_api(REFLECTION) Meta::Value
    quickParse(minitl::Allocator& arena, const char* strBegin, const char* strEnd = 0);
}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
