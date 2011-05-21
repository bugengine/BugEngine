/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_MESH_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_MESH_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

be_tag(ResourceLoaders())
class be_api(GRAPHICS) Mesh : public Resource
{
public:
    Mesh();
    ~Mesh();
};

}}

/*****************************************************************************/
#endif
