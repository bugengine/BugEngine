/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_PRIMITIVE_STATICMESH_HH_
#define BE_GRAPHICS_PRIMITIVE_STATICMESH_HH_
/*****************************************************************************/
#include    <graphics/primitive/primitive.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT StaticMesh : public Primitive
{
public:
    StaticMesh();
    ~StaticMesh();

    be_metaclass(GRAPHICSEXPORT,StaticMesh,Primitive)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
