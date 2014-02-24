/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SHADERMODEL1_FLOAT_FLOAT_SCRIPT_HH_
#define BE_SHADERMODEL1_FLOAT_FLOAT_SCRIPT_HH_
/**************************************************************************************************/
#include    <shadermodel1/stdafx.h>
#include    <3d/shader/types.script.hh>

namespace BugEngine { namespace Float { namespace Float
{

class Constant : public Shaders::Float
{
published:
    const float value;
published:
    Constant(float value);
    ~Constant();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream,
                                   Shaders::Stage currentStage,
                                   Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream,
                                  Shaders::Stage currentStage,
                                  Shaders::Stage targetStage) const override;
};


class Uniform : public Shaders::Float
{
published:
    const istring name;
published:
    Uniform(istring name);
    ~Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream,
                                   Shaders::Stage currentStage,
                                   Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream,
                                  Shaders::Stage currentStage,
                                  Shaders::Stage targetStage) const override;
};


class Varying : public Shaders::Float
{
published:
    Varying();
    ~Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream,
                                   Shaders::Stage currentStage,
                                   Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream,
                                  Shaders::Stage currentStage,
                                  Shaders::Stage targetStage) const override;
};

}}}

/**************************************************************************************************/
#endif
