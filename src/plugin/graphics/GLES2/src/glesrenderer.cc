/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glesrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <system/scheduler/scheduler.hh>

namespace BugEngine { namespace OpenGLES
{

GLESRenderer::GLESRenderer(const PluginContext& context)
    :   IRenderer(Arena::general(), context.resourceManager, Scheduler::MainThread)
{
}

GLESRenderer::~GLESRenderer()
{
}

void GLESRenderer::flush()
{
    IRenderer::flush();
}

ref<IGPUResource> GLESRenderer::create(weak<const RenderSurface> /*rendersurface*/) const
{
    return ref<IGPUResource>();
}

ref<IGPUResource> GLESRenderer::create(weak<const RenderWindow> /*renderwindow*/) const
{
    return ref<IGPUResource>();
}

ref<IGPUResource> GLESRenderer::create(weak<const ShaderProgram> /*shader*/) const
{
    return ref<IGPUResource>();
}

uint2 GLESRenderer::getScreenSize() const
{
    return make_uint2(320, 200);
}

}}