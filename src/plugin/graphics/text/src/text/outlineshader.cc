/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <text/outlineshader.script.hh>
#include    <3d/shader/types.script.hh>

namespace BugEngine
{

minitl::vector< ref<Shaders::Output> > createOutlineShader(weak<Shaders::Float4> textColor,
                                                           weak<Shaders::Float4> backgroundColor)
{
    be_forceuse(textColor);
    be_forceuse(backgroundColor);
    minitl::vector< ref<Shaders::Output> > result(Arena::temporary());
    result.reserve(1);
    //result.push_back();
    return result;
}

OutlineShader::OutlineShader(weak<Shaders::Float4> textColor,
                             weak<Shaders::Float4> backgroundColor)
    :   ShaderProgramDescription(createOutlineShader(textColor, backgroundColor))
{
}

OutlineShader::~OutlineShader()
{
}

}

