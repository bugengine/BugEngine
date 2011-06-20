/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_LOADERS_DX9SHADER_HH_
#define BE_DX9_LOADERS_DX9SHADER_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>
#include    <dx9renderer.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Dx9ShaderProgram : public IGPUResource
{
private:
    void load(weak<const Resource> resource) override;
    void unload() override;
public:
    Dx9ShaderProgram(weak<const ShaderProgram> resource, weak<Dx9Renderer> renderer);
    ~Dx9ShaderProgram();
};


class Dx9Shader : public IGPUResource
{
private:
    void load(weak<const Resource> resource) override;
    void unload() override;
public:
    Dx9Shader(weak<const Shader> resource, weak<Dx9Renderer> renderer);
    ~Dx9Shader();
};

}}}

/*****************************************************************************/
#endif
