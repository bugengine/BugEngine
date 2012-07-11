/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_LOADERS_DX9SHADER_HH_
#define BE_DX9_LOADERS_DX9SHADER_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>
#include    <dx9renderer.hh>
#include    <d3d9.h>

namespace BugEngine { namespace DirectX9
{

class Dx9ShaderProgram : public IGPUResource
{
private:
    void load(weak<const Resource> resource) override;
    void unload() override;
public:
    Dx9ShaderProgram(weak<const ShaderProgram> resource, weak<const Dx9Renderer> renderer);
    ~Dx9ShaderProgram();
};

}}

/*****************************************************************************/
#endif
