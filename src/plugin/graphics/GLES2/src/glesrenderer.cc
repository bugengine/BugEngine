/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glesrenderer.hh>
#include    <3d/rendertarget/rendertarget.script.hh>
#include    <scheduler/scheduler.hh>

namespace BugEngine { namespace OpenGLES
{

GLESRenderer::GLESRenderer(const Plugin::Context& context)
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

ref<IGPUResource> GLESRenderer::create(weak<const RenderSurfaceDescription> /*renderSurfaceDescription*/) const
{
    return ref<IGPUResource>();
}

ref<IGPUResource> GLESRenderer::create(weak<const RenderWindowDescription> /*renderWindowDescription*/) const
{
    return ref<IGPUResource>();
}

ref<IGPUResource> GLESRenderer::create(weak<const ShaderProgramDescription> /*shaderDescription*/) const
{
    return ref<IGPUResource>();
}

uint2 GLESRenderer::getScreenSize() const
{
    return make_uint2(320, 200);
}

}}
