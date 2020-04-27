/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_STDAFX_H_
#define BE_RTTI_STDAFX_H_
/**************************************************************************************************/

#include    <core/stdafx.h>
#include    <kernel/simd.hh>
#include    <kernel/colors.hh>


#if defined(building_rtti)
# define     BE_API_RTTI        BE_EXPORT
#elif defined(be_dll_rtti)
# define     BE_API_RTTI        BE_IMPORT
#else
# define     BE_API_RTTI
#endif

#define     published           public
#define     be_tag(v)

#ifndef BE_BUILD_KERNEL
namespace BugEngine { namespace Arena
{
be_api(RTTI) minitl::Allocator& rtti();
be_api(RTTI) minitl::Allocator& script();

}}

namespace BugEngine { namespace RTTI
{

struct Class;
struct Type;
class Value;

}}

#include <rtti/builtin.hh>
#endif


/**************************************************************************************************/
#endif
