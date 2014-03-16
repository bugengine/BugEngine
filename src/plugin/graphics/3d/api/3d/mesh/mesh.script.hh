/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_MESH_MESH_SCRIPT_HH_
#define BE_3D_MESH_MESH_SCRIPT_HH_
/**************************************************************************************************/
#include    <3d/stdafx.h>
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(_3D) MeshDescription : public Resource::Description
{
public:
    MeshDescription();
    ~MeshDescription();
};

}

/**************************************************************************************************/
#endif
