/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_TEXTURE_HH_
#define BE_GRAPHICS_MATERIAL_TEXTURE_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <graphics/material/texturepipeline.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT _Texture
{
public:
    typedef TexturePipeline  Pipeline;
public:
    _Texture();
    virtual ~_Texture();
};

typedef Resource<_Texture>   Texture;

}}

/*****************************************************************************/
#endif
