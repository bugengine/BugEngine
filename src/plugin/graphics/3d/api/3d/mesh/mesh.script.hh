/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_MESH_MESH_SCRIPT_HH_
#define BE_3D_MESH_MESH_SCRIPT_HH_
/*****************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(_3D) Mesh : public Resource::Description
{
public:
    Mesh();
    ~Mesh();
};

}

/*****************************************************************************/
#endif
