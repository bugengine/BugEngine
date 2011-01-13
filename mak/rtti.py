import os

class Container:
	def __init__(self, parent, name, line, scope):
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
		self.scope = scope
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
		Container.__init__(self, None, '', 1, 'public')
		self.source = source

	def dump(self, file, namespace = '', index = 0):
		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % self.source)
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/namespace.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/enuminfo.script.hh>\n")
		file.write("#include    <rtti/engine/getset.hh>\n")
		file.write("\n")
		#file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		index = Container.dump(self, file, namespace, index)
		file.write("namespace BugEngine {\n\n")
		for fullname,classname in self.classes:
			file.write("    template< > ref<const RTTI::ClassInfo> const be_typeid< %s >::klass() { return %sClass(); }\n" % (fullname, classname))
		_, _4 = os.path.split(self.source)
		_, _3 = os.path.split(_)
		_, _2 = os.path.split(_)
		_, _1 = os.path.split(_)
		identifier = _1+'_'+_2+'_'+_3+'_'+_4
		identifier = ''.join([i for i in identifier if i.isalpha()])
		file.write("    struct %s { %s(); ~%s(); };\n" % (identifier, identifier, identifier))
		file.write("    %s::%s()\n" % (identifier, identifier))
		file.write("    {\n")
		file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		for fullname,classname in self.classes:
			name = fullname[2:].replace('::', '.')
			file.write("        {\n")
			file.write("            inamespace name = inamespace(\"%s\");\n" % (name))
			file.write("            weak<const RTTI::ClassInfo> cs = %sClass();\n" % (classname))
			file.write("            ns->add(name, Value(cs));\n")
			file.write("            %sRegisterProperties();\n" % classname)
			file.write("        }\n")
		file.write("    }\n")
		file.write("    %s::~%s()\n" % (identifier, identifier))
		file.write("    {\n")
		file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		for fullname,classname in self.classes[::-1]:
			name = fullname[2:].replace('::', '.')
			file.write("        {\n")
			file.write("            %sUnregisterProperties();\n" % classname)
			file.write("            inamespace name = inamespace(\"%s\");\n" % (name))
			file.write("            ns->remove(name);\n")
			file.write("        }\n")
		file.write("    }\n")
		file.write("    BE_EXPORT %s s_%s;\n" % (identifier, identifier))
		file.write("}\n")
		return index

class Typedef(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

	def dump(self, file, namespace, index):
		#file.write("#line %d\n" % self.line)
		file.write("namespace %s {\n" % self.name)
		index = Container.dump(self, file, namespace + '::' + self.name, index)
		file.write("}\n")
		self.parent.classes += self.classes
		return index

class Enum(Container):
	def __init__(self, parent, name, line, scope):
		Container.__init__(self, parent, name, line, scope)

	def dump(self, file, namespace, index):
		if self.scope != 'public':
			return
		decl = "enum%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl))
		#file.write("#line %d\n" % (self.line))
		file.write("    static inline ref< ::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    { static ref< ::BugEngine::RTTI::ClassInfo> klass = ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< void >::klass(), ref< ::BugEngine::RTTI::ClassInfo>(), sizeof(%s), 0); return klass; }\n" % (self.fullname[2:].replace('::', '.'), self.fullname))
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		file.write("    }\n")
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line, scope):
		Container.__init__(self, parent, name, line, scope)
		self.inherits = inherits or 'void'

	def dump(self, file, namespace, index):
		if self.scope != 'public':
			return
		decl = "class%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl))
		file.write("    static inline minitl::ref< ::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    {\n")
		file.write("        ::BugEngine::Logger::root();\n")
		file.write("        static minitl::ref< ::BugEngine::RTTI::ClassInfo> klass;\n")
		file.write("        if(klass)\n")
		file.write("            return klass;\n")
		if self.inherits != 'void':
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent = ::BugEngine::be_typeid< %s >::klass();\n" % self.inherits)
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(rttiArena(), ::BugEngine::inamespace(\"%s\"), parent->metaclass);\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		else:
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent;\n")
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< ::BugEngine::RTTI::ClassInfo>::klass());\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		file.write("        klass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent, metaclass, sizeof(%s), (i32)(ptrdiff_t)static_cast<%s*>((%s*)1)-1);\n" % (self.fullname[2:].replace('::', '.'), self.fullname, self.inherits, self.fullname))
		#file.write("#line %d\n" % (self.line))
		file.write("        return klass;\n");
		file.write("    }\n")
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		for type,name,visibility,line in self.members:
			if visibility == 'public':
				#file.write("#line %d\n" % (line))
				file.write("        klass->addProperty(\"%s\", minitl::ref< ::BugEngine::RTTI::PropertyInfo>::create(::BugEngine::rttiArena(), BugEngine::be_typeid< %s >::type()));\n" % (name, type))
		file.write("    }\n")
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		for type,name,visibility,line in self.members:
			if visibility == 'public':
				#file.write("#line %d\n" % (line))
				file.write("        klass->removeProperty(\"%s\");\n" % (name))
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

