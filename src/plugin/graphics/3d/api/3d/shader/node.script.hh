/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SHADER_NODE_SCRIPT_HH_
#define BE_3D_SHADER_NODE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace Shaders
{

class IShaderBuilder;

class be_api(_3D) Node : public minitl::refcountable
{
    BE_NOCOPY(Node);
public:

protected:
    Node();
    ~Node();
public:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const = 0;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const = 0;
};

}}

/*****************************************************************************/
#endif
