/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADERPIPELINE_HH_
#define BE_GRAPHICS_MATERIAL_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <system/resource/pipeline.hh>
#include    <graphics/material/shaderparam.hh>

namespace BugEngine { namespace Graphics
{

class Shader;

class ShaderPipeline : public System::Pipeline
{
public:
    virtual Shader*             load(const ifilename& file) = 0;
    virtual void                unload(Shader* s) = 0;

    virtual ShaderParam::Type   getTypeByName(const char *type) = 0;
    virtual const char *        getTypeName(ShaderParam::Type type) = 0;
};

}}

/*****************************************************************************/
#endif
