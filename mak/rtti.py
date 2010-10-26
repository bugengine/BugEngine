
class Container:
	def __init__(self, parent, name, line):
		self.parent = parent
		self.name = name
		if parent:
			self.fullname = parent.fullname
			if name:
				self.fullname += '::' + name
			self.parent.addObject(self)
		else:
			self.fullname = ''
		self.objects = []
		self.members = []
		self.line = line
		self.classes = []

	def addObject(self, object):
		self.objects.append(object)

	def addMember(self, type, name):
		self.members.append((type, name))

	def dump(self, file, namespace, index):
		for o in self.objects:
			index = o.dump(file, namespace, index)
		return index

class Root(Container):
	def __init__(self, source):
		Container.__init__(self, None, '', 1)
		self.source = source

	def dump(self, file, namespace = '', index = 0):
		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % self.source)
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/engine/classinfo.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/enuminfo.script.hh>\n")
		file.write("#include    <rtti/engine/wrapper.hh>\n")
		file.write("\n")
		file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		index = Container.dump(self, file, namespace, index)
		file.write("namespace BugEngine {\n\n")
		for fullname,classname in self.classes:
			file.write("    template< > const RTTI::ClassInfo* const be_typeid< %s >::klass = &%s;\n" % (fullname, classname))
		file.write("}\n")
		return index

class Typedef(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

	def dump(self, file, namespace, index):
		file.write("#line %d\n" % self.line)
		file.write("namespace %s {\n\n" % self.name)
		index = Container.dump(self, file, namespace + '::' + self.name, index)
		file.write("}\n")
		self.parent.classes += self.classes
		return index

class Enum(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

	def dump(self, file, namespace, index):
		file.write("")
		decl = "class%d" % index
		self.classes.append((self.fullname, namespace + '::s_' + decl + "Class"))
		file.write("#line %d\n" % (self.line))
		file.write("    static const char * const s_%sName = \"%s\";\n" % (decl, self.fullname))
		file.write("#line %d\n" % (self.line))
		file.write("    static const ::BugEngine::RTTI::ClassInfo s_%sClass = { { s_%sName }, { ::BugEngine::be_typeid< void >::klass }, { 0 }, { 0 }, { 0 }, sizeof(%s), 0, { 0 } };\n" % (decl, decl, self.fullname))
		file.write("    static const ::BugEngine::RTTI::ClassInfoRegistration s_%sClassRegistration (&s_%sClass);\n" % (decl, decl))
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line):
		Container.__init__(self, parent, name, line)
		self.inherits = inherits or 'void'

	def dump(self, file, namespace, index):
		file.write("")
		decl = "class%d" % index
		self.classes.append((self.fullname, namespace + '::s_' + decl + "Class"))
		if self.members:
			file.write("#line %d\n" % (self.line))
			file.write("    static const ::BugEngine::RTTI::PropertyInfo s_%sProperties[] = {\n" % decl)
			index = 0
			for type,name in self.members:
				file.write("        { { \"%s\" }, { { ::BugEngine::be_typeid< ::BugEngine::RTTI::RefType< %s >::Type >::klass } , ::BugEngine::TypeInfo::Type(::BugEngine::RTTI::RefType< %s >::Reference), ::BugEngine::TypeInfo::Constness(::BugEngine::RTTI::RefType< %s >::Constness) }, &::BugEngine::RTTI::get< %s, %s, &%s::%s >, &::BugEngine::RTTI::set< %s, %s, &%s::%s > }" % (name, type, type, type, type, self.fullname, self.fullname, name, type, self.fullname, self.fullname, name))
				index += 1
				if index < len(self.members):
					file.write(",\n")
				else:
					file.write("\n")
			file.write("    };\n")
			props = "s_%sProperties" % decl
			propCount = "(sizeof(s_%sProperties)/sizeof(s_%sProperties[0]))" % (decl, decl)
		else:
			props = "0"
			propCount = 0
		file.write("#line %d\n" % (self.line))
		file.write("    static const char * const s_%sName = \"%s\";\n" % (decl, self.fullname))
		file.write("#line %d\n" % (self.line))
		file.write("    static const ::BugEngine::RTTI::ClassInfo s_%sClass = { { s_%sName }, { ::BugEngine::be_typeid< %s >::klass }, { 0 }, { 0 }, { 0 }, sizeof(%s), %s, { %s } };\n" % (decl, decl, self.inherits, self.fullname, propCount, props))
		file.write("    static const ::BugEngine::RTTI::ClassInfoRegistration s_%sClassRegistration (&s_%sClass);\n" % (decl, decl))
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

