#!python


hHeader = """/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_%(TYPEUPPER)s_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_%(TYPEUPPER)s_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Shaders { namespace %(TYPECAML)s
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

#include    <%(PROJECT)s/stdafx.h>
#include    <%(PROJECT)s/shaders/%(TYPE)s.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Shaders {namespace %(TYPECAML)s
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
"""ref<%(TYPE1)s> operator %(OPERATOR)s(weak<const %(TYPE2)s> node1, weak<const %(TYPE3)s> node2);
""",
"""class %(TYPE2)s%(OPNAME)s%(TYPE3)s : public %(TYPE1)s
{
private:
    weak<const %(TYPE2)s> node1;
    weak<const %(TYPE3)s> node2;
public:
    %(TYPE2)s%(OPNAME)s%(TYPE3)s(weak<const %(TYPE2)s> node1, weak<const %(TYPE3)s> node2)
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
            stream.addOperator(this, Op_%(OPNAME)s, Type_%(TYPE1)s, node1, node2);
        }
    }
};
ref<%(TYPE1)s> operator %(OPERATOR)s(weak<const %(TYPE2)s> node1, weak<const %(TYPE3)s> node2)
{
    return ref<%(TYPE2)s%(OPNAME)s%(TYPE3)s>::create(Arena::script(), node1, node2);
}

"""
]










def header(h, cpp, n, project):
	h.write(hHeader % {'TYPEUPPER': n.upper(), 'TYPE': n.lower(), 'TYPECAML':n, 'PROJECT':project })
	cpp.write(cppHeader % {'TYPE': n.lower(), 'TYPECAML': n, 'PROJECT':project })

def footer(h, cpp):
	h.write(hFooter)
	cpp.write(cppFooter)

operators = [
 ('/', 'div'),
 ('+', 'add'),
 ('-', 'sub')
]

def predecl(h, basetype, col, row):
	if col == 1 and row == 1:
		type = "%s" % basetype
	elif row == 1:
		type = "%s%d" % (basetype, col)
	else:
		type = "%s%dx%d" % (basetype, col, row)
	h.write("class %s;\n" % type)

def fileType(h, cpp, basetype, col, row, doMatrix):
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
		h.write(operator[0] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':type, 'OPERATOR':op, 'OPNAME':opname})
		cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':type, 'OPERATOR':op, 'OPNAME':opname})
	if col > 1 and row > 1:
		for l in [2, 3, 4]:
			type1 = "%s%dx%d" % (basetype, l, row)
			type2 = "%s%dx%d" % (basetype, col, l)
			h.write(operator[0] % {'TYPE1':type, 'TYPE2':type1, 'TYPE3':type2, 'OPERATOR':'*', 'OPNAME':'mul'})
			cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type1, 'TYPE3':type2, 'OPERATOR':'*', 'OPNAME':'mul'})
	elif col > 1 and doMatrix:
		type2 = "%s%dx%d" % (basetype, col, col)
		h.write(operator[0] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':type2, 'OPERATOR':'*', 'OPNAME':'mul'})
		cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':type2, 'OPERATOR':'*', 'OPNAME':'mul'})
		h.write(operator[0] % {'TYPE1':type, 'TYPE2':type2, 'TYPE3':type, 'OPERATOR':'*', 'OPNAME':'mul'})
		cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type2, 'TYPE3':type, 'OPERATOR':'*', 'OPNAME':'mul'})
	if col > 1:
		h.write(operator[0] % {'TYPE1':type, 'TYPE2':basetype, 'TYPE3':type, 'OPERATOR':'*', 'OPNAME':'mul'})
		cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':basetype, 'TYPE3':type, 'OPERATOR':'*', 'OPNAME':'mul'})
		h.write(operator[0] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':basetype, 'OPERATOR':'*', 'OPNAME':'mul'})
		cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':basetype, 'OPERATOR':'*', 'OPNAME':'mul'})
	for (op, opname) in operators:
		if col > 1:
			h.write(operator[0] % {'TYPE1':type, 'TYPE2':basetype, 'TYPE3':type, 'OPERATOR':op, 'OPNAME':opname})
			cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':basetype, 'TYPE3':type, 'OPERATOR':op, 'OPNAME':opname})
			h.write(operator[0] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':basetype, 'OPERATOR':op, 'OPNAME':opname})
			cpp.write(operator[1] % {'TYPE1':type, 'TYPE2':type, 'TYPE3':basetype, 'OPERATOR':op, 'OPNAME':opname})



	h.write("\n\n\n")
	cpp.write("\n\n\n")




types = [
	("float", "Float", True, True, "shadermodel1"),
	("double", "Double", True, True, "shadermodel4"),
	("int", "Int", True, False, "shadermodel1"),
	("uint", "Uint", True, False, "shadermodel1"),
	("bool", "Bool", True, False, "shadermodel1")
]

for rawtype, shadertype, doVector, doMatrix, shadermodel in types:
	h = open("src/plugin/graphics/%s/api/%s/shaders/%s.script.hh" % (shadermodel, shadermodel, rawtype), "w")
	cpp = open("src/plugin/graphics/%s/src/shaders/%s.cc" % (shadermodel, rawtype), "w")
	header(h, cpp, shadertype, shadermodel)
	fileType(h, cpp, shadertype, 1, 1, doMatrix)
	if doVector:
		predecl(h, shadertype, 2, 1)
		predecl(h, shadertype, 3, 1)
		predecl(h, shadertype, 4, 1)
	if doMatrix:
		predecl(h, shadertype, 2, 2)
		predecl(h, shadertype, 3, 2)
		predecl(h, shadertype, 4, 2)
		predecl(h, shadertype, 2, 3)
		predecl(h, shadertype, 3, 3)
		predecl(h, shadertype, 4, 3)
		predecl(h, shadertype, 2, 4)
		predecl(h, shadertype, 3, 4)
		predecl(h, shadertype, 4, 4)
	if doVector:
		fileType(h, cpp, shadertype, 2, 1, doMatrix)
		fileType(h, cpp, shadertype, 3, 1, doMatrix)
		fileType(h, cpp, shadertype, 4, 1, doMatrix)
	if doMatrix:
		fileType(h, cpp, shadertype, 2, 2, doMatrix)
		fileType(h, cpp, shadertype, 3, 2, doMatrix)
		fileType(h, cpp, shadertype, 4, 2, doMatrix)
		fileType(h, cpp, shadertype, 2, 3, doMatrix)
		fileType(h, cpp, shadertype, 3, 3, doMatrix)
		fileType(h, cpp, shadertype, 4, 3, doMatrix)
		fileType(h, cpp, shadertype, 2, 4, doMatrix)
		fileType(h, cpp, shadertype, 3, 4, doMatrix)
		fileType(h, cpp, shadertype, 4, 4, doMatrix)
	footer(h, cpp)

