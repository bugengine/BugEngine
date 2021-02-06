/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_STDAFX_H_
#define BE_RTTI_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/kernel/colors.hh>
#include <bugengine/kernel/simd.hh>

#if defined(building_rtti)
#    define BE_API_RTTI BE_EXPORT
#elif defined(be_dll_rtti)
#    define BE_API_RTTI BE_IMPORT
#else
#    define BE_API_RTTI
#endif

#define published public
#define be_tag(v)

#ifndef BE_COMPUTE
namespace BugEngine { namespace Arena {
be_api(RTTI) minitl::Allocator& rtti();
be_api(RTTI) minitl::Allocator& script();

}}  // namespace BugEngine::Arena

namespace BugEngine { namespace RTTI {

struct Class;
struct Type;
class Value;

}}  // namespace BugEngine::RTTI

#    include <bugengine/rtti/builtin.hh>
#endif

/**************************************************************************************************/
#endif
