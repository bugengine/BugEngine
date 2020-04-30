/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_VALUEPARSE_HH_
#define BE_RTTIPARSE_VALUEPARSE_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/rtti/value.hh>
#include <bugengine/rttiparse/dbcontext.hh>
#include <bugengine/rttiparse/dbnamespace.hh>
#include <bugengine/rttiparse/node.hh>
#include <bugengine/rttiparse/object.hh>

namespace BugEngine { namespace RTTI {

be_api(RTTIPARSE) ref< Parser::Node > parseValue(minitl::Allocator&   arena,
                                                 Parser::MessageList& context, const char* strBegin,
                                                 const char* strEnd = 0, u32 initialLine = 0,
                                                 u32 initialColumn = 0);
be_api(RTTIPARSE) ref< Parser::Object > parseObject(minitl::Allocator&   arena,
                                                    Parser::MessageList& context,
                                                    const char* strBegin, const char* strEnd = 0,
                                                    u32 initialLine = 0, u32 initialColumn = 0);

be_api(RTTIPARSE) RTTI::Value
   quickParse(minitl::Allocator& arena, const char* strBegin, const char* strEnd = 0);
}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
