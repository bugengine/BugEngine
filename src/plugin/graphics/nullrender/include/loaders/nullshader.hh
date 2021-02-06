/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLSHADER_HH_
#define BE_NULLRENDER_LOADERS_NULLSHADER_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>

namespace BugEngine { namespace Null {

class NullRenderer;

class NullShaderProgram : public IGPUResource
{
public:
    NullShaderProgram(weak< const ShaderProgramDescription > shaderDescription,
                      weak< const NullRenderer >             renderer);
    ~NullShaderProgram();

private:
    void load(weak< const Resource::Description > shaderDescription) override;
    void unload() override;
};

}}  // namespace BugEngine::Null

/**************************************************************************************************/
#endif
