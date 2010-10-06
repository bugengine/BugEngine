
class Container:
	def __init__(self):
		self.objects = []

	def __init__(self, parent, name, line):
		self.parent = parent
		if parent:
			self.namespace = parent.namespace
			if name:
				self.namespace += '::' + name
			self.parent.addObject(self)
		else:
			self.namespace = ''
		self.objects = []
		self.line = line

	def addObject(self, object):
		self.objects.append(object)

	def dump(self, file):
		for o in self.objects:
			o.dump(file)
			file.write('\n')

class Root(Container):
	def __init__(self, source):
		Container.__init__(self, None, '', 1)
		self.source = source

	def dump(self, file):
		file.write("#include    <%s>\n" % self.source)
		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/classinfo.hh>\n")
		file.write("\n")
		file.write("namespace BugEngine {\n\n")
		file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		Container.dump(self, file)
		file.write("}\n")

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

class Class(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

	def dump(self, file):
		file.write("")
		file.write("#line %d\n" % (self.line))
		file.write("    template< > const char *be_typeid<%s>::name = \"%s\";\n" % (self.namespace,self.namespace))
		file.write("#line %d\n" % (self.line))
		file.write("    template< > const RTTI::ClassInfo be_typeid<%s>::klass = { name, 0, sizeof(%s), 0, 0, 0, 0, 0, 0, 0, 0 };\n" % (self.namespace,self.namespace))
		Container.dump(self, file)
