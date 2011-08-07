#!python


hHeader = """/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_%(TYPEUPPER)s_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_%(TYPEUPPER)s_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

"""

hFooter = """
}}}
/*****************************************************************************/
#endif
"""

cppHeader = """/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/%(TYPE)s.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
"""


cppFooter = """}}}
"""




exporttype = [
"""class %(TYPE)s : public Node
{
    BE_NOCOPY(%(TYPE)s)
protected:
    inline %(TYPE)s();
    inline ~%(TYPE)s();
};

""",
"""/* Type *************************************************************/
%(TYPE)s::%(TYPE)s()
{
}
%(TYPE)s::~%(TYPE)s()
{
}

"""
]

uniform = [
"""class %(TYPE)sUniform : public %(TYPE)s
{
    BE_NOCOPY(%(TYPE)sUniform)
published:
    const istring name;
published:
    %(TYPE)sUniform(const istring& name);
    ~%(TYPE)sUniform();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

""",
"""/* Uniform **********************************************************/
%(TYPE)sUniform::%(TYPE)sUniform(const istring& name)
    :   name(name)
{
}
%(TYPE)sUniform::~%(TYPE)sUniform()
{
}
void %(TYPE)sUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_%(TYPE)s);
}
void %(TYPE)sUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

"""
]


attribute = [
"""class %(TYPE)sAttribute : public %(TYPE)s
{
    BE_NOCOPY(%(TYPE)sAttribute)
published:
    %(TYPE)sAttribute();
    ~%(TYPE)sAttribute();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

""",
"""/* Attribute ********************************************************/
%(TYPE)sAttribute::%(TYPE)sAttribute()
{
}
%(TYPE)sAttribute::~%(TYPE)sAttribute()
{
}
void %(TYPE)sAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_%(TYPE)s);
}
void %(TYPE)sAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

"""
]



varying = [
"""class %(TYPE)sVarying : public %(TYPE)s
{
    BE_NOCOPY(%(TYPE)sVarying)
published:
    const weak<const %(TYPE)s> node;
published:
    %(TYPE)sVarying(weak<const %(TYPE)s> node);
    ~%(TYPE)sVarying();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

""",
"""/* Varying **********************************************************/
%(TYPE)sVarying::%(TYPE)sVarying(weak<const %(TYPE)s> node)
    :   node(node)
{
}
%(TYPE)sVarying::~%(TYPE)sVarying()
{
}
void %(TYPE)sVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_%(TYPE)s);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void %(TYPE)sVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

"""
]



operator = [
"""ref<%(TYPE)s> operator %(OPERATOR)s(weak<const %(TYPE)s> node1, weak<const %(TYPE)s> node2);
""",
"""class %(TYPE)s%(OPNAME)s : public %(TYPE)s
{
private:
    weak<const %(TYPE)s> node1;
    weak<const %(TYPE)s> node2;
public:
    %(TYPE)s%(OPNAME)s(weak<const %(TYPE)s> node1, weak<const %(TYPE)s> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_%(OPNAME)s, Type_%(TYPE)s, node1, node2);
        }
    }
};
ref<%(TYPE)s> operator %(OPERATOR)s(weak<const %(TYPE)s> node1, weak<const %(TYPE)s> node2)
{
    return ref<%(TYPE)s%(OPNAME)s>::create(gameArena(), node1, node2);
}

"""
]










def header(h, cpp, n):
	h.write(hHeader % {'TYPEUPPER': n.upper(), 'TYPE': n })
	cpp.write(cppHeader % {'TYPE': n })

def footer(h, cpp):
	h.write(hFooter)
	cpp.write(cppFooter)

operators = [
 ('*', 'mul'),
 ('/', 'div'),
 ('+', 'add'),
 ('-', 'sub')
]

def fileType(h, cpp, basetype, col, row):
	if col == 1 and row == 1:
		type = "%s" % basetype
	elif row == 1:
		type = "%s%d" % (basetype, col)
	else:
		type = "%s%dx%d" % (basetype, col, row)

	h.write(exporttype[0] % {'TYPE': type})
	cpp.write(exporttype[1] % {'TYPE': type})
	h.write(uniform[0] % {'TYPE': type})
	cpp.write(uniform[1] % {'TYPE': type})
	h.write(attribute[0] % {'TYPE': type})
	cpp.write(attribute[1] % {'TYPE': type})
	h.write(varying[0] % {'TYPE': type})
	cpp.write(varying[1] % {'TYPE': type})
	for (op, opname) in operators:
		h.write(operator[0] % {'TYPE':type, 'OPERATOR':op, 'OPNAME':opname})
		cpp.write(operator[1] % {'TYPE':type, 'OPERATOR':op, 'OPNAME':opname})
	h.write("\n\n\n")
	cpp.write("\n\n\n")




types = [
	("float", "Float", True, True),
	("double", "Double", True, True),
	("int", "Int", True, False),
	("uint", "Uint", True, False),
	("bool", "Bool", True, False)
]

for rawtype, shadertype, doVector, doMatrix in types:
	h = open("src/engine/graphics/api/graphics/objects/shaders/%s.script.hh" % rawtype, "w")
	cpp = open("src/engine/graphics/src/objects/shaders/%s.cc" % rawtype, "w")
	header(h, cpp, rawtype)
	fileType(h, cpp, shadertype, 1, 1)
	if doVector:
		fileType(h, cpp, shadertype, 2, 1)
		fileType(h, cpp, shadertype, 3, 1)
		fileType(h, cpp, shadertype, 4, 1)
	if doMatrix:
		fileType(h, cpp, shadertype, 2, 2)
		fileType(h, cpp, shadertype, 3, 2)
		fileType(h, cpp, shadertype, 4, 2)
		fileType(h, cpp, shadertype, 2, 3)
		fileType(h, cpp, shadertype, 3, 3)
		fileType(h, cpp, shadertype, 4, 3)
		fileType(h, cpp, shadertype, 2, 4)
		fileType(h, cpp, shadertype, 3, 4)
		fileType(h, cpp, shadertype, 4, 4)
	footer(h, cpp)

