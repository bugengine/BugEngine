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
    minitl::vector<Namespace>   m_namespaces;
    Namespace                   m_input;
    Namespace                   m_output;
    MemoryStream                m_stream;
    i32                         m_indent;
    u32                         m_counter;
public:
    IShaderBuilder();

    const char *text() const;
    i64         textSize() const;
public:
    void addUniform(weak<const Node> node, Stage stage, const istring& name, Type type);
    void addVarying(weak<const Node> node, Stage stage, const istring& name, Type type);
protected:
    istring referenceNode(weak<const Node> node);
    virtual void doAddUniformDeclaration(const istring& name, Stage stage, Type type) = 0;
    virtual void doAddVaryingDeclaration(const istring& name, Stage stage, Type type) = 0;
protected:
    virtual ~IShaderBuilder();
protected:
    void indent();
    void unindent();
    void write(const char *text);
};

}}}

/*****************************************************************************/
#endif
