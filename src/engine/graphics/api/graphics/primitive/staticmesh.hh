/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_PRIMITIVE_STATICMESH_HH_
#define BE_GRAPHICS_PRIMITIVE_STATICMESH_HH_
/*****************************************************************************/
#include    <graphics/primitive/primitive.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) StaticMesh : public Primitive
{
public:
    StaticMesh();
    ~StaticMesh();
};

}}

/*****************************************************************************/
#endif
