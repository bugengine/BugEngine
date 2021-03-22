/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_TAGS_EDITOR_SCRIPT_HH_
#define BE_META_TAGS_EDITOR_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>

namespace BugEngine { namespace EditHint {

struct be_api(META) Extension
{
    const istring ext;
    Extension(const istring& ext) : ext(ext)
    {
    }
};

struct be_api(META) Temporary {Temporary() {}};

struct be_api(META) OutputNode {OutputNode() {}};

}}  // namespace BugEngine::EditHint

/**************************************************************************************************/
#endif
