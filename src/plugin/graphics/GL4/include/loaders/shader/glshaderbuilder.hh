/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_GLSHADERBUILDER_HH_
#define BE_GL4_LOADERS_GLSHADERBUILDER_HH_
/*****************************************************************************/
#include    <3d/shader/ishaderbuilder.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace OpenGL
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
    virtual void doAddUniformDeclaration(const istring& name, Shaders::Stage stage, Shaders::ValueType type) override;
    virtual void doAddVaryingDeclaration(const istring& name, Shaders::Stage stage, Shaders::ValueType type) override;
    virtual void doAddAttributeDeclaration(const istring& name, Shaders::Stage stage, Shaders::ValueType type) override;
    virtual void doAddOperator(Shaders::Operator op, Shaders::ValueType type, const istring& result, const istring& op1, const istring& op2) override;
    virtual void doAddMethod(const istring& name) override;
    virtual void doEndMethod() override;
    virtual void doSaveTo(Shaders::Semantic semantic, const istring& value) override;
    virtual void doSaveTo(const istring& name, const istring& value) override;
};

}}

/*****************************************************************************/
#endif
