/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_EXTENSIONS_HH_
#define BE_OPENGL_EXTENSIONS_HH_
/*****************************************************************************/

namespace BugEngine { namespace Graphics { namespace OpenGL
{

typedef void(*Extension)();
Extension glGetExtension(const char *name);

struct ShaderExtensions
{
    typedef GLhandleARB (*glCreateShaderObjectARBProc)(GLenum);
    typedef void (*glShaderSourceARBProc)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *);
    typedef void (*glCompileShaderARBProc)(GLhandleARB);
    typedef void (*glGetObjectParameterivARBProc)(GLhandleARB, GLenum, GLint*);
    typedef void (*glGetInfoLogARBProc)(GLhandleARB, GLsizei, GLsizei*, GLcharARB*);

    const glCreateShaderObjectARBProc   glCreateShader;
    const glShaderSourceARBProc         glShaderSource;
    const glCompileShaderARBProc        glCompileShader;
    const glGetObjectParameterivARBProc glGetObjectParameteriv;
    const glGetInfoLogARBProc           glGetInfoLog;
    ShaderExtensions();

private:
    ShaderExtensions& operator=(const ShaderExtensions&);
};

}}}

/*****************************************************************************/
#endif
