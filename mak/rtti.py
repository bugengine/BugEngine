
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
		self.visibility = 'public'

	def addObject(self, object):
		self.objects.append(object)

	def addMember(self, type, name, line):
		self.members.append((type, name, self.visibility, line))

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
			file.write("    template< > ref<const RTTI::ClassInfo> const be_typeid< %s >::klass() { return %s(); }\n" % (fullname, classname))
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
		file.write("namespace %s {\n" % self.name)
		index = Container.dump(self, file, namespace + '::' + self.name, index)
		file.write("}\n")
		self.parent.classes += self.classes
		return index

class Enum(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line)

	def dump(self, file, namespace, index):
		file.write("")
		decl = "enum%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl + "Class"))
		file.write("#line %d\n" % (self.line))
		file.write("    static inline ref<::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    { static ref<::BugEngine::RTTI::ClassInfo> klass = ref<::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< void >::klass(), ref<::BugEngine::RTTI::ClassInfo>(), sizeof(%s), 0); return klass; }\n" % (self.fullname[2:].replace('::', '.'), self.fullname))
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line):
		Container.__init__(self, parent, name, line)
		self.inherits = inherits or 'void'

	def dump(self, file, namespace, index):
		file.write("")
		decl = "class%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl + "Class"))
		file.write("    static inline ref<::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    {\n")
		file.write("        static ref<::BugEngine::RTTI::ClassInfo> klass;\n")
		file.write("        if(!klass)\n")
		file.write("        {\n")
		file.write("#line %d\n" % (self.line))
		file.write("            klass = ref<::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< %s >::klass(), ref<::BugEngine::RTTI::ClassInfo>(), sizeof(%s), (i32)(ptrdiff_t)static_cast<%s*>((%s*)0));\n" % (self.fullname[2:].replace('::', '.'), self.inherits, self.fullname, self.inherits, self.fullname))
		for type,name,visibility,line in self.members:
			if visibility == 'be_published':
				file.write("#line %d\n" % (line))
				type = "::BugEngine::TypeInfo(::BugEngine::be_typeid< ::BugEngine::RTTI::RefType< %s >::Type >::klass(), ::BugEngine::TypeInfo::Type(::BugEngine::RTTI::RefType< %s >::Reference), ::BugEngine::TypeInfo::Constness(::BugEngine::RTTI::RefType< %s >::Constness))" % (type, type, type)
				file.write("            klass->addProperty(\"%s\", ref<::BugEngine::RTTI::PropertyInfo>::create(::BugEngine::rttiArena(), %s));\n" % (name, type))
				#file.write("        { { \"%s\" }, { { ::BugEngine::be_typeid< ::BugEngine::RTTI::RefType< %s >::Type >::klass } , ::BugEngine::TypeInfo::Type(::BugEngine::RTTI::RefType< %s >::Reference), ::BugEngine::TypeInfo::Constness(::BugEngine::RTTI::RefType< %s >::Constness) }, &::BugEngine::RTTI::get< %s, %s, &%s::%s >, &::BugEngine::RTTI::set< %s, %s, &%s::%s > }" % (name, type, type, type, type, self.fullname, self.fullname, name, type, self.fullname, self.fullname, name))
		file.write("        }\n");
		file.write("    return klass;\n");
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

