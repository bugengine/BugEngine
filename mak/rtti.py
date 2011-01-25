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
		self.methods = {}
		self.line = line
		self.classes = []
		self.scope = scope
		self.visibility = 'public'

	def addObject(self, object):
		self.objects.append(object)

	def addMember(self, type, attr, name, line):
		self.members.append((type, name, attr, self.visibility, line))

	def addMethod(self, name, attr, rtype, params, line):
		if self.visibility == 'public':
			if self.methods.has_key(name):
				self.methods[name].append((rtype, params, attr, self.visibility, line))
			else:
				self.methods[name] = [(rtype, params, attr, self.visibility, line)]

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
		file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		index = Container.dump(self, file, namespace, index)
		file.write("namespace BugEngine {\n\n")
		for fullname,classname in self.classes:
			file.write("    template< > ref<RTTI::ClassInfo> be_typeid< %s >::klassBuilder() { return %sClass(); }\n" % (fullname, classname))
		_, _4 = os.path.split(self.source)
		_, _3 = os.path.split(_)
		_, _2 = os.path.split(_)
		_, _1 = os.path.split(_)
		identifier = _1+'_'+_2+'_'+_3+'_'+_4
		identifier = ''.join([i for i in identifier if i.isalpha()])

		for fullname,classname in self.classes:
			name = fullname[2:].replace('::', '.')
			file.write("    template< > be_typeid< %s >::PropertyBuilder::PropertyBuilder()\n" % (fullname))
			file.write("    {\n")
			file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
			file.write("        {\n")
			file.write("            inamespace name = inamespace(\"%s\");\n" % (name))
			file.write("            weak<const RTTI::ClassInfo> cs = %sClass();\n" % (classname))
			file.write("            ns->add(name, Value(cs));\n")
			file.write("            %sRegisterProperties();\n" % classname)
			file.write("        }\n")
			file.write("    }\n")

		for fullname,classname in self.classes[::-1]:
			name = fullname[2:].replace('::', '.')
			file.write("    template< > be_typeid< %s >::PropertyBuilder::~PropertyBuilder()\n" % (fullname))
			file.write("    {\n")
			file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
			file.write("        {\n")
			file.write("            %sUnregisterProperties();\n" % classname)
			file.write("            inamespace name = inamespace(\"%s\");\n" % (name))
			file.write("            ns->remove(name);\n")
			file.write("        }\n")
			file.write("    }\n")
			file.write("    template< > be_typeid< %s >::PropertyBuilder be_typeid< %s >::s_properties;\n" % (fullname, fullname))

		file.write("}\n")
		return index

class Typedef(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

	def dump(self, file, namespace, index):
		file.write("#line %d\n" % self.line)
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
		file.write("#line %d\n" % (self.line))
		file.write("    static inline ref< ::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    { static ref< ::BugEngine::RTTI::ClassInfo> klass = ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< void >::klass(), ref< ::BugEngine::RTTI::ClassInfo>(), be_checked_numcast<u32>(sizeof(%s)), 0); return klass; }\n" % (self.fullname[2:].replace('::', '.'), self.fullname))
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
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent->metaclass);\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		else:
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent;\n")
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< ::BugEngine::RTTI::ClassInfo>::klass());\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		file.write("#line %d\n" % (self.line))
		file.write("        klass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent, metaclass, be_checked_numcast<u32>(sizeof(%s)), be_checked_numcast<i32>((ptrdiff_t)static_cast<%s*>((%s*)1)-1));\n" % (self.fullname[2:].replace('::', '.'), self.fullname, self.inherits, self.fullname))
		file.write("        return klass;\n");
		file.write("    }\n")
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		for type,name,attr,visibility,line in self.members:
			if visibility == 'public':
				file.write("#line %d\n" % (line))
				getter = '&::BugEngine::RTTI::get< %s, %s, &%s::%s >' % (type, self.fullname, self.fullname, name)
				if attr.find('const') == -1:
					setter = ', &::BugEngine::RTTI::set< %s, %s, &%s::%s >' % (type, self.fullname, self.fullname, name)
				else:
					setter = ''
				file.write("        klass->addProperty(\"%s\", minitl::ref< ::BugEngine::RTTI::PropertyInfo>::create(::BugEngine::rttiArena(), BugEngine::be_typeid< %s >::type(), %s %s));\n" % (name, type, getter, setter))
		for name, overloads in self.methods.iteritems():
			file.write("        {\n")
			file.write("            ref< ::BugEngine::RTTI::MethodInfo > mi = minitl::ref< ::BugEngine::RTTI::MethodInfo >::create(::BugEngine::rttiArena());\n")
			file.write("            mi->overloads.reserve(%d);\n" % len(overloads))
			for rtype, params, attrs, visibility, line in overloads:
				print rtype, name, params, attrs
				file.write("#line %d\n" % (line))
				file.write("            mi->overloads.push_back(::BugEngine::RTTI::OverloadInfo(::BugEngine::be_typeid< %s >::type()));\n" % rtype)
				for ptype, pname in params:
					file.write("            mi->overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"%s\", ::BugEngine::be_typeid< %s >::type()));\n" % (pname, ptype))
			file.write("        }\n")
		file.write("    }\n")
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo > klass = s_%sClass();\n" % decl)
		for type,name,attr,visibility,line in self.members:
			if visibility == 'public':
				file.write("#line %d\n" % (line))
				file.write("        klass->removeProperty(\"%s\");\n" % (name))
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1)
		self.parent.classes += self.classes
		return index

