/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_PARSE_VALUEPARSE_HH_
#define BE_RTTI_PARSE_VALUEPARSE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-parse/stdafx.h>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/dbnamespace.hh>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI {

be_api(RTTI_PARSE) ref< AST::Node > parseValue(minitl::Allocator& arena, AST::MessageList& context,
                                              const char* strBegin, const char* strEnd = 0,
                                              u32 initialLine = 0, u32 initialColumn = 0);
be_api(RTTI_PARSE) ref< AST::Object > parseObject(minitl::Allocator& arena,
                                                 AST::MessageList& context, const char* strBegin,
                                                 const char* strEnd = 0, u32 initialLine = 0,
                                                 u32 initialColumn = 0);

be_api(RTTI_PARSE) RTTI::Value
    quickParse(minitl::Allocator& arena, const char* strBegin, const char* strEnd = 0);
}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
