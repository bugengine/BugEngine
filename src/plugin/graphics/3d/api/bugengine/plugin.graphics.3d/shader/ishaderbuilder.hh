/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_SHADER_ISHADERBUILDER_HH_
#define BE_3D_SHADER_ISHADERBUILDER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/core/memory/streams.hh>
#include <bugengine/minitl/hash_map.hh>
#include <bugengine/plugin.graphics.3d/shader/node.script.hh>

namespace BugEngine { namespace Shaders {

enum Semantic
{
    Position,
    Color,
    Depth
};

enum ValueType
{
    Type_Float,
    Type_Float2,
    Type_Float3,
    Type_Float4,
    Type_Float2x2,
    Type_Float2x3,
    Type_Float2x4,
    Type_Float3x2,
    Type_Float3x3,
    Type_Float3x4,
    Type_Float4x2,
    Type_Float4x3,
    Type_Float4x4,

    Type_Double,
    Type_Double2,
    Type_Double3,
    Type_Double4,
    Type_Double2x2,
    Type_Double2x3,
    Type_Double2x4,
    Type_Double3x2,
    Type_Double3x3,
    Type_Double3x4,
    Type_Double4x2,
    Type_Double4x3,
    Type_Double4x4,

    Type_Int,
    Type_Int2,
    Type_Int3,
    Type_Int4,
    Type_Uint,
    Type_Uint2,
    Type_Uint3,
    Type_Uint4,

    Type_Bool,
    Type_Bool2,
    Type_Bool3,
    Type_Bool4,

    Type_Sampler1D,
    Type_Sampler2D,
    Type_Sampler3D,
    Type_SamplerCube
};

enum Operator
{
    Op_mul = '*',
    Op_div = '/',
    Op_add = '+',
    Op_sub = '-'
};

enum Builtin
{
    Builtin_mul
};

class be_api(3D) IShaderBuilder
{
    BE_NOCOPY(IShaderBuilder);

private:
    struct Namespace
    {
        Namespace();
        minitl::hashmap< weak< const Node >, istring > names;
    };
    enum ShaderConstants
    {
        AttributeMax = 16,
        VaryingMax   = 16
    };
    minitl::vector< Namespace >                         m_namespaces;
    size_t                                              m_currentAttribute;
    size_t                                              m_currentVarying;
    size_t                                              m_currentAttributeToVarying;
    size_t                                              m_currentTemporary;
    minitl::vector< minitl::tuple< istring, istring > > m_attributeToVarying;
    MemoryStream                                        m_stream;
    i32                                                 m_indent;
    u32                                                 m_counter;

public:
    IShaderBuilder();

    const char* text() const;
    i64         textSize() const;

public:
    void addUniform(weak< const Node > node, Stage stage, const istring& name, ValueType type);
    void addVarying(weak< const Node > node, Stage currentStage, Stage targetStage, ValueType type);
    void addAttribute(weak< const Node > node, Stage currentStage, Stage targetStage,
                      ValueType type);
    void forwardAttributes();
    void beginMethodDefinition(const istring& name);
    void end();
    void saveTo(Semantic semantic, weak< const Node > node);
    void addOperator(weak< const Node > node, Operator op, ValueType type, weak< const Node > node1,
                     weak< const Node > node2);
    void write(float value);
    void write(float2 value);
    void write(float3 value);
    void write(float4 value);
    void write(int value);
    void write(int2 value);
    void write(int3 value);
    void write(int4 value);
    void write(bool value);

protected:
    istring referenceNode(weak< const Node > node);

    virtual void doAddUniformDeclaration(const istring& name, Stage stage, ValueType type)   = 0;
    virtual void doAddVaryingDeclaration(const istring& name, Stage stage, ValueType type)   = 0;
    virtual void doAddAttributeDeclaration(const istring& name, Stage stage, ValueType type) = 0;
    virtual void doAddMethod(const istring& name)                                            = 0;
    virtual void doEndMethod()                                                               = 0;
    virtual void doWrite(float value)                                                        = 0;
    virtual void doWrite(float2 value)                                                       = 0;
    virtual void doWrite(float3 value)                                                       = 0;
    virtual void doWrite(float4 value)                                                       = 0;
    virtual void doWrite(int value)                                                          = 0;
    virtual void doWrite(int2 value)                                                         = 0;
    virtual void doWrite(int3 value)                                                         = 0;
    virtual void doWrite(int4 value)                                                         = 0;
    virtual void doWrite(bool value)                                                         = 0;

    virtual void doSaveTo(Semantic semantic, const istring& expr)     = 0;
    virtual void doSaveTo(const istring& target, const istring& expr) = 0;

    virtual void doAddOperator(Operator op, ValueType type, const istring& result,
                               const istring& op1, const istring& op2)
       = 0;

protected:
    virtual ~IShaderBuilder();

protected:
    void indent();
    void unindent();
    void write(const char* text);
    void writeln(const char* text);
};

}}  // namespace BugEngine::Shaders

/**************************************************************************************************/
#endif
