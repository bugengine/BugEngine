/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLSHADER_HH_
#define BE_NULLRENDER_LOADERS_NULLSHADER_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <3d/renderer/igpuresource.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace Null
{

class NullRenderer;

class NullShaderProgram : public IGPUResource
{
public:
    NullShaderProgram(weak<const ShaderProgramDescription> shaderDescription, weak<const NullRenderer> renderer);
    ~NullShaderProgram();

private:
    void load(weak<const Resource::Description> shaderDescription) override;
    void unload() override;
};

}}

/**************************************************************************************************/
#endif
