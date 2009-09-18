/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <graphics/material/shaderpipeline.hh>

namespace BugEngine { namespace Graphics
{

class ShaderParam;

class GRAPHICSEXPORT _Shader
{
public:
    typedef ShaderPipeline  Pipeline;
public:
    _Shader();
    virtual ~_Shader();

    virtual ShaderParam* getParam(const istring& name) const = 0;
};

typedef Resource<_Shader>   Shader;

}}

/*****************************************************************************/
#endif
