/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
#define BE_OPENGL_LOADERS_GLSHADERBUILDER_HH_
/*****************************************************************************/
#include    <graphics/tools/ishaderbuilder.hh>
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

struct GLShaderBuilder : public Shaders::IShaderBuilder
{
private:
    GLenum  m_shaderType;
public:
    GLShaderBuilder(GLenum shaderType);
    ~GLShaderBuilder();

    istring attributes[64];
private:
    virtual void doAddUniformDeclaration(const istring& name, Shaders::Stage stage, Shaders::Type type) override;
    virtual void doAddVaryingDeclaration(const istring& name, Shaders::Stage stage, Shaders::Type type) override;
};

}}}

/*****************************************************************************/
#endif
