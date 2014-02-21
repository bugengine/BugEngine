/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <shadermodel1/stdafx.h>
#include    <shadermodel1/float/float.script.hh>

namespace BugEngine { namespace Float { namespace Float
{

Float::Float()
{
}

Float::~Float()
{
}

Constant::Constant(float value)
    :   value(value)
{
}

Constant::~Constant()
{
}

void Constant::buildDeclarations(Shaders::IShaderBuilder& stream,
                                 Shaders::Stage currentStage,
                                 Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

void Constant::buildDefinitions(Shaders::IShaderBuilder& stream,
                                Shaders::Stage currentStage,
                                Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

Varying::Varying()
{
}

Varying::~Varying()
{
}

void Varying::buildDeclarations(Shaders::IShaderBuilder& stream,
                                Shaders::Stage currentStage,
                                Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

void Varying::buildDefinitions(Shaders::IShaderBuilder& stream,
                               Shaders::Stage currentStage,
                               Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

Uniform::Uniform(istring name)
    :   name(name)
{
}

Uniform::~Uniform()
{
}

void Uniform::buildDeclarations(Shaders::IShaderBuilder& stream,
                                Shaders::Stage currentStage,
                                Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

void Uniform::buildDefinitions(Shaders::IShaderBuilder& stream,
                               Shaders::Stage currentStage,
                               Shaders::Stage targetStage) const
{
    be_forceuse(stream);
    be_forceuse(currentStage);
    be_forceuse(targetStage);
}

}}}