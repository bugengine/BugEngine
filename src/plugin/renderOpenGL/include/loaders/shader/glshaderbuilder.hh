/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
#define BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
/*****************************************************************************/
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

struct GLShaderBuilder : public Shaders::IShaderBuilder
{
public:
    GLShaderBuilder();
    ~GLShaderBuilder();

private:
    virtual void doAddUniformDeclaration(const istring& name, Shaders::Type type) override;
};

}}}

/*****************************************************************************/
#endif
