/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_LOADERS_NULLSHADER_HH_
#define BE_RENDERNULL_LOADERS_NULLSHADER_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics { namespace Null
{

class NullRenderer;

class NullShaderProgram : public IGPUResource
{
public:
    NullShaderProgram(weak<const ShaderProgram> resource, weak<NullRenderer> renderer);
    ~NullShaderProgram();

private:
    void load(weak<const Resource> resource) override;
    void unload() override;
};

class NullShader : public IGPUResource
{
public:
    NullShader(weak<const Shader> resource, weak<NullRenderer> renderer);
    ~NullShader();

private:
    void load(weak<const Resource> resource) override;
    void unload() override;
};

}}}

/*****************************************************************************/
#endif
