
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
		self.line = line
		self.classes = []

	def addObject(self, object):
		self.objects.append(object)

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
		file.write("\n")
		file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		index = Container.dump(self, file, namespace, index)
		file.write("namespace BugEngine {\n\n")
		for fullname,classname in self.classes:
			file.write("    template< > const RTTI::ClassInfo* const be_typeid<%s>::klass = &%s;\n" % (fullname, classname))
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

class Class(Container):
	def __init__(self, parent, name, inherits, line):
		Container.__init__(self, parent, name, line)
		self.inherits = inherits or 'void'

	def dump(self, file, namespace, index):
		file.write("")
		decl = "class%d" % index
		self.classes.append((self.fullname, namespace + '::s_' + decl + "Class"))
		file.write("#line %d\n" % (self.line))
		file.write("    static const char * const s_%sName = \"%s\";\n" % (decl, self.fullname))
		file.write("#line %d\n" % (self.line))
		file.write("    static const RTTI::ClassInfo s_%sClass = { s_%sName, ::BugEngine::be_typeid<%s>::klass, 0, sizeof(%s) };\n" % (decl, decl, self.inherits, self.fullname))
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index
