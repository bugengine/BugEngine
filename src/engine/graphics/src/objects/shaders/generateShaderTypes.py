#!python

def header(f, cpp, n):
	f.write("/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>\n")
	f.write("   see LICENSE for detail */\n")
	f.write("/* GENERATED FILE! do not edit; see generateShaderTypes.py */\n")
	f.write("\n")
	f.write("#ifndef BE_GRAPHICS_OBJECTS_SHADERS_%s_SCRIPT_HH_\n" % n.upper())
	f.write("#define BE_GRAPHICS_OBJECTS_SHADERS_%s_SCRIPT_HH_\n" % n.upper())
	f.write("/*****************************************************************************/\n")
	f.write("#include    <graphics/objects/shaders/node.script.hh>\n")
	f.write("\n")
	f.write("namespace BugEngine { namespace Graphics { namespace Shaders\n")
	f.write("{\n\n")

	cpp.write("/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>\n")
	cpp.write("   see LICENSE for detail */\n")
	cpp.write("/* GENERATED FILE! do not edit; see generateShaderTypes.py */\n")
	cpp.write("\n")
	cpp.write("#include    <graphics/stdafx.h>\n")
	cpp.write("#include    <graphics/objects/shaders/%s.script.hh>\n" % n)
	cpp.write("\n")
	cpp.write("namespace BugEngine { namespace Graphics { namespace Shaders\n")
	cpp.write("{\n\n")

def footer(f, cpp):
	f.write("\n")
	f.write("}}}\n")
	f.write("/*****************************************************************************/\n")
	f.write("#endif\n")

	cpp.write("}}}\n")


def fileType(h, cpp, basetype, col, row):
	if col == 1 and row == 1:
		type = "%s" % basetype
	elif row == 1:
		type = "%s%d" % (basetype, col)
	else:
		type = "%s%dx%d" % (basetype, col, row)

	h.write("class %s : public Node\n" % type)
	h.write("{\n")
	h.write("    BE_NOCOPY(%s)\n" % type)
	h.write("protected:\n")
	h.write("    inline %s();\n" % type)
	h.write("    inline ~%s();\n" % type)
	h.write("};\n\n")
	h.write("class %sUniform : public %s\n" % (type,type))
	h.write("{\n")
	h.write("    BE_NOCOPY(%sUniform)\n" % type)
	h.write("published:\n")
	h.write("    const istring name;\n")
	h.write("published:\n")
	h.write("    %sUniform(const istring& name);\n" % type)
	h.write("    ~%sUniform();\n" % type)
	h.write("private:\n")
	h.write("    virtual void buildDeclarations(IShaderBuilder& stream) const override;\n")
	h.write("    virtual void buildDefinitions(IShaderBuilder& stream) const override;\n")
	h.write("};\n\n")

	cpp.write("%s::%s()\n" % (type, type))
	cpp.write("{\n")
	cpp.write("}\n\n")
	cpp.write("%s::~%s()\n" % (type, type))
	cpp.write("{\n")
	cpp.write("}\n\n")
	cpp.write("%sUniform::%sUniform(const istring& name)\n" % (type, type))
	cpp.write("    :   name(name)\n")
	cpp.write("{\n")
	cpp.write("}\n\n")
	cpp.write("%sUniform::~%sUniform()\n" % (type, type))
	cpp.write("{\n")
	cpp.write("}\n\n")
	cpp.write("void %sUniform::buildDeclarations(IShaderBuilder& /*stream*/) const\n" % type)
	cpp.write("{\n")
	cpp.write("}\n\n")
	cpp.write("void %sUniform::buildDefinitions(IShaderBuilder& /*stream*/) const\n" % type)
	cpp.write("{\n")
	cpp.write("}\n\n")


types = [("float", "Float", True, True),
("double", "Double", True, True),
("int", "Int", True, False),
("uint", "Uint", True, False),
("bool", "Bool", True, False)]

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

