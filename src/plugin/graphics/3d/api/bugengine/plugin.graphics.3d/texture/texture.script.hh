/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
#define BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

class be_api(3D) TextureDescription : public Resource::Description
{
public:
    TextureDescription();
    ~TextureDescription();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
