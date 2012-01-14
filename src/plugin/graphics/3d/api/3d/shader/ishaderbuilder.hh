/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SHADER_ISHADERBUILDER_HH_
#define BE_3D_SHADER_ISHADERBUILDER_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>
#include    <3d/shader/node.script.hh>


namespace BugEngine { namespace Shaders
{

enum Semantic
{
    Position,
    Color,
    Depth
};

enum Type
{
    Type_Float, Type_Float2, Type_Float3, Type_Float4,
    Type_Float2x2, Type_Float2x3, Type_Float2x4,
    Type_Float3x2, Type_Float3x3, Type_Float3x4,
    Type_Float4x2, Type_Float4x3, Type_Float4x4,

    Type_Double, Type_Double2, Type_Double3, Type_Double4,
    Type_Double2x2, Type_Double2x3, Type_Double2x4,
    Type_Double3x2, Type_Double3x3, Type_Double3x4,
    Type_Double4x2, Type_Double4x3, Type_Double4x4,

    Type_Int, Type_Int2, Type_Int3, Type_Int4,
    Type_Uint, Type_Uint2, Type_Uint3, Type_Uint4,

    Type_Bool, Type_Bool2, Type_Bool3, Type_Bool4,

    Type_Sampler1D, Type_Sampler2D, Type_Sampler3D, Type_SamplerCube
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

class be_api(_3D) IShaderBuilder
{
    BE_NOCOPY(IShaderBuilder);
private:
    struct Namespace
    {
        Namespace();
        minitl::hashmap< weak<const Node>, istring >   names;
    };
    enum ShaderConstants { AttributeMax = 16, VaryingMax = 16 };
    minitl::vector<Namespace>                           m_namespaces;
    size_t                                              m_currentAttribute;
    size_t                                              m_currentVarying;
    size_t                                              m_currentAttributeToVarying;
    size_t                                              m_currentTemporary;
    minitl::vector< minitl::pair<istring, istring> >    m_attributeToVarying;
    MemoryStream                                        m_stream;
    i32                                                 m_indent;
    u32                                                 m_counter;
public:
    IShaderBuilder();

    const char *text() const;
    i64         textSize() const;
public:
    void addUniform(weak<const Node> node, Stage stage, const istring& name, Type type);
    void addVarying(weak<const Node> node, Stage currentStage, Stage targetStage, Type type);
    void addAttribute(weak<const Node> node, Stage currentStage, Stage targetStage, Type type);
    void forwardAttributes();
    void beginMethodDefinition(const istring& name);
    void end();
    void saveTo(Semantic semantic, weak<const Node> node);
    void addOperator(weak<const Node> node, Operator op, Type type, weak<const Node> node1, weak<const Node> node2);
protected:
    istring referenceNode(weak<const Node> node);

    virtual void doAddUniformDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddVaryingDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddAttributeDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddMethod(const istring& name) = 0;
    virtual void doEndMethod() = 0;

    virtual void doSaveTo(Semantic semantic, const istring& expr) = 0;
    virtual void doSaveTo(const istring& target, const istring& expr) = 0;

    virtual void doAddOperator(Operator op, Type type, const istring& result, const istring& op1, const istring& op2) = 0;
protected:
    virtual ~IShaderBuilder();
protected:
    void indent();
    void unindent();
    void write(const char *text);
    void writeln(const char *text);
};

}}

/*****************************************************************************/
#endif
