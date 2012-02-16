/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_LOADERS_NULLSHADER_HH_
#define BE_RENDERNULL_LOADERS_NULLSHADER_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace Null
{

class NullRenderer;

class NullShaderProgram : public IGPUResource
{
public:
    NullShaderProgram(weak<const ShaderProgram> resource, weak<const NullRenderer> renderer);
    ~NullShaderProgram();

private:
    void load(weak<const Resource> resource) override;
    void unload() override;
};

}}

/*****************************************************************************/
#endif
