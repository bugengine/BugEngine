/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <extensions.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

ShaderExtensions::ShaderExtensions()
    :   glCreateShader((glCreateShaderObjectARBProc)glGetExtension("glCreateShaderObjectARB"))
    ,   glShaderSource((glShaderSourceARBProc)glGetExtension("glShaderSourceARB"))
    ,   glCompileShader((glCompileShaderARBProc)glGetExtension("glCompileShaderARB"))
{
}

}}}
