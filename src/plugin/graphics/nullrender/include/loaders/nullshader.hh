/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_NULLRENDER_LOADERS_NULLSHADER_HH_
#define BE_NULLRENDER_LOADERS_NULLSHADER_HH_
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
