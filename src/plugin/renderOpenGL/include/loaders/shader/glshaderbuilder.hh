/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
#define BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
/*****************************************************************************/
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

struct GLShaderBuilder : public IShaderBuilder
{
public:
    GLShaderBuilder();
    ~GLShaderBuilder();
};

}}}

/*****************************************************************************/
#endif
