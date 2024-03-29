/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_EXTENSIONS_HH_
#define BE_GL4_EXTENSIONS_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>

namespace BugEngine { namespace OpenGL {

typedef void (*Extension)();
Extension glGetExtension(const char* name);

struct ShaderExtensions
{
    typedef GLhandleARB(APIENTRY* glCreateProgramObjectARBProc)();
    typedef void(APIENTRY* glDeleteProgramObjectARBProc)(GLhandleARB);
    typedef void(APIENTRY* glAttachObjectARBProc)(GLhandleARB, GLhandleARB);
    typedef void(APIENTRY* glDetachObjectARBProc)(GLhandleARB, GLhandleARB);
    typedef void(APIENTRY* glLinkProgramARBProc)(GLhandleARB);

    typedef GLhandleARB(APIENTRY* glCreateShaderObjectARBProc)(GLenum);
    typedef void(APIENTRY* glDeleteShaderObjectARBProc)(GLhandleARB);
    typedef void(APIENTRY* glShaderSourceARBProc)(GLhandleARB, GLsizei, const GLcharARB**,
                                                  const GLint*);
    typedef void(APIENTRY* glCompileShaderARBProc)(GLhandleARB);
    typedef void(APIENTRY* glGetObjectParameterivARBProc)(GLhandleARB, GLenum, GLint*);
    typedef void(APIENTRY* glGetInfoLogARBProc)(GLhandleARB, GLsizei, GLsizei*, GLcharARB*);

    const glCreateProgramObjectARBProc glCreateProgram;
    const glDeleteProgramObjectARBProc glDeleteProgram;
    const glAttachObjectARBProc        glAttachShader;
    const glDetachObjectARBProc        glDetachShader;
    const glLinkProgramARBProc         glLinkProgram;

    const glCreateShaderObjectARBProc   glCreateShader;
    const glDeleteShaderObjectARBProc   glDeleteShader;
    const glShaderSourceARBProc         glShaderSource;
    const glCompileShaderARBProc        glCompileShader;
    const glGetObjectParameterivARBProc glGetObjectParameteriv;
    const glGetInfoLogARBProc           glGetInfoLog;
    ShaderExtensions();

private:
    ShaderExtensions& operator=(const ShaderExtensions&);
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
