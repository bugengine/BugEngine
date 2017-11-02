/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <scheduler.hh>


namespace BugEngine { namespace Kernel { namespace OpenCL_GL
{

minitl::array<cl_context_properties> Scheduler::createPlatformSpecificContextProperties()
{
    HGLRC glrc = wglGetCurrentContext();
    HDC dc = wglGetCurrentDC();
    if (glrc)
    {
        minitl::array<cl_context_properties> properties(Arena::temporary(), 5);
        properties[0] = CL_GL_CONTEXT_KHR;
        properties[1] = (cl_context_properties) glrc;
        properties[2] = CL_WGL_HDC_KHR;
        properties[3] = (cl_context_properties) dc;
        properties[4] = 0;
        return properties;
    }
    else
    {
        be_info("no OpenGL context found; OpenGL/OpenCL compatibility disabled");
        minitl::array<cl_context_properties> properties(Arena::temporary(), 1);
        properties[0] = 0;
        return properties;
    }
}

}}}
