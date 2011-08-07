/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_ISHADERBUILDER_HH_
#define BE_GRAPHICS_MATERIAL_ISHADERBUILDER_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class be_api(GRAPHICS) IShaderBuilder
{
    BE_NOCOPY(IShaderBuilder);
private:
    struct Namespace
    {
        Namespace();
        minitl::hashmap< weak<const Node>, istring >    names;
    };
    enum ShaderConstants { AttributeMax = 16, VaryingMax = 16 };
    minitl::vector<Namespace>                           m_namespaces;
    size_t                                              m_currentAttribute;
    size_t                                              m_currentVarying;
    size_t                                              m_currentAttributeToVarying;
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
protected:
    istring referenceNode(weak<const Node> node);
    virtual void doAddUniformDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddVaryingDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddAttributeDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddMethod(const istring& name) = 0;
    virtual void doEndMethod() = 0;
    virtual void doSaveTo(Semantic semantic, const istring& value) = 0;
    virtual void doSaveTo(const istring& target, const istring& value) = 0;
protected:
    virtual ~IShaderBuilder();
protected:
    void indent();
    void unindent();
    void write(const char *text);
    void writeln(const char *text);
};

}}}

/*****************************************************************************/
#endif
