/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_VALUEPARSE_HH_
#define BE_RTTIPARSE_VALUEPARSE_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rtti/value.hh>
#include    <rttiparse/dbcontext.hh>
#include    <rttiparse/dbnamespace.hh>
#include    <rttiparse/node.hh>
#include    <rttiparse/object.hh>


namespace BugEngine { namespace RTTI
{

be_api(RTTI)
ref<Parser::Node> parseValue(minitl::Allocator& arena,
                             Parser::ErrorList& errors,
                             const char* strBegin,
                             const char *strEnd = 0,
                             u32 initialLine = 0,
                             u32 initialColumn = 0);
be_api(RTTI)
ref<Parser::Object> parseObject(minitl::Allocator& arena,
                                Parser::ErrorList& errors,
                                const char* strBegin,
                                const char *strEnd = 0,
                                u32 initialLine = 0,
                                u32 initialColumn = 0);

}}

/**************************************************************************************************/
#endif

