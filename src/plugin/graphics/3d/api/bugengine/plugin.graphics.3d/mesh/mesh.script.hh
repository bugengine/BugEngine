/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_MESH_MESH_SCRIPT_HH_
#define BE_3D_MESH_MESH_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

class be_api(3D) MeshDescription : public Resource::Description
{
public:
    MeshDescription();
    ~MeshDescription();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
