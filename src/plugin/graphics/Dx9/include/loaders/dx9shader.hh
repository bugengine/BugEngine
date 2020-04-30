/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_DX9_LOADERS_DX9SHADER_HH_
#define BE_DX9_LOADERS_DX9SHADER_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <d3d9.h>
#include <dx9renderer.hh>

namespace BugEngine { namespace DirectX9 {

class Dx9ShaderProgram : public IGPUResource
{
private:
    void load(weak< const Resource::Description > resource) override;
    void unload() override;

public:
    Dx9ShaderProgram(weak< const ShaderProgramDescription > shaderDescription,
                     weak< const Dx9Renderer >              renderer);
    ~Dx9ShaderProgram();
};

}}  // namespace BugEngine::DirectX9

/**************************************************************************************************/
#endif
