/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_STDAFX_H_
#define BE_META_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>
#include <bugengine/kernel/colors.hh>
#include <bugengine/kernel/simd.hh>

#if defined(building_meta)
#    define BE_API_META BE_EXPORT
#elif defined(be_dll_meta)
#    define BE_API_META BE_IMPORT
#else
#    define BE_API_META
#endif

#define published public
#define be_tag(v)

#ifndef BE_COMPUTE
namespace BugEngine { namespace Arena {
be_api(META) minitl::Allocator& meta();
be_api(META) minitl::Allocator& script();

}}  // namespace BugEngine::Arena

namespace BugEngine { namespace Meta {

struct Class;
struct Type;
class Value;

}}  // namespace BugEngine::Meta

#    include <bugengine/meta/builtin.hh>
#endif

/**************************************************************************************************/
#endif
