/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <extensions.hh>

namespace BugEngine { namespace OpenGL
{

ShaderExtensions::ShaderExtensions()
    :   glCreateProgram((glCreateProgramObjectARBProc)glGetExtension("glCreateProgramObjectARB"))
    ,   glDeleteProgram((glDeleteProgramObjectARBProc)glGetExtension("glDeleteObjectARB"))
    ,   glAttachShader((glAttachObjectARBProc)glGetExtension("glAttachObjectARB"))
    ,   glDetachShader((glDetachObjectARBProc)glGetExtension("glDetachObjectARB"))
    ,   glLinkProgram((glLinkProgramARBProc)glGetExtension("glLinkProgramARB"))
    ,   glCreateShader((glCreateShaderObjectARBProc)glGetExtension("glCreateShaderObjectARB"))
    ,   glDeleteShader((glDeleteShaderObjectARBProc)glGetExtension("glDeleteObjectARB"))
    ,   glShaderSource((glShaderSourceARBProc)glGetExtension("glShaderSourceARB"))
    ,   glCompileShader((glCompileShaderARBProc)glGetExtension("glCompileShaderARB"))
    ,   glGetObjectParameteriv((glGetObjectParameterivARBProc)glGetExtension("glGetObjectParameterivARB"))
    ,   glGetInfoLog((glGetInfoLogARBProc)glGetExtension("glGetInfoLogARB"))
{
}

}}
