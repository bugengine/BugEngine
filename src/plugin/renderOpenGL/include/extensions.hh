/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_EXTENSIONS_HH_
#define BE_OPENGL_EXTENSIONS_HH_
/*****************************************************************************/

namespace BugEngine { namespace Graphics { namespace OpenGL
{

void* glGetExtension(const char *name);

struct ShaderExtensions
{
    typedef GLhandleARB (*glCreateShaderObjectARBProc)(GLenum);
    typedef void (*glShaderSourceARBProc)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *);
    typedef void (*glCompileShaderARBProc)(GLhandleARB);

    const glCreateShaderObjectARBProc   glCreateShader;
    const glShaderSourceARBProc         glShaderSource;
    const glCompileShaderARBProc        glCompileShader;
    ShaderExtensions();
};

}}}

/*****************************************************************************/
#endif
