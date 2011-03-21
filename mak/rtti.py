import os


showline = 0

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

	def addMember(self, type, attr, name, tags, line):
		self.members.append((type, name, attr, self.visibility, tags, line))

	def addMethod(self, name, attr, rtype, params, tags, line):
		if self.visibility == 'published':
			if self.methods.has_key(name):
				self.methods[name].append((rtype, params, attr, self.visibility, tags, line))
			else:
				self.methods[name] = [(rtype, params, attr, self.visibility, tags, line)]

	def dump(self, file, namespace, index, nested):
		for o in self.objects:
			if o.scope == 'public' or o.scope == 'published':
				index = o.dump(file, namespace, index, nested)
		return index

class Root(Container):
	def __init__(self, source):
		Container.__init__(self, None, '', 1, 'public')
		self.source = source

	def dump(self, file, namespace = '', index = 0, nested = False):
		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % self.source)
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/typeinfo.inl>\n")
		file.write("#include    <rtti/value.hh>\n")
		file.write("#include    <rtti/value.inl>\n")
		file.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/namespace.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/helper/get.hh>\n")
		file.write("#include    <rtti/engine/helper/set.hh>\n")
		file.write("#include    <rtti/engine/helper/method.hh>\n")
		file.write("\n")
		if showline: file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		index = Container.dump(self, file, namespace, index, False)
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
			file.write("        weak<const RTTI::ClassInfo> cs = %sClass();\n" % (classname))
			file.write("        %sRegisterProperties();\n" % classname)
			file.write("    }\n")

		for fullname,classname in self.classes[::-1]:
			name = fullname[2:].replace('::', '.')
			file.write("    template< > be_typeid< %s >::PropertyBuilder::~PropertyBuilder()\n" % (fullname))
			file.write("    {\n")
			file.write("        weak<RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
			file.write("        inamespace name = inamespace(\"%s\");\n" % (name))
			file.write("        ns->remove(name);\n")
			file.write("        %sUnregisterProperties();\n" % classname)
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

	def dump(self, file, namespace, index, nested):
		if showline: file.write("#line %d\n" % self.line)
		file.write("namespace %s {\n" % self.name)
		index = Container.dump(self, file, namespace + '::' + self.name, index, False)
		file.write("}\n")
		self.parent.classes += self.classes
		return index

class Enum(Container):
	def __init__(self, parent, name, line, scope):
		Container.__init__(self, parent, name, line, scope)
		self.values = []
		self.tags = []

	def addEnumValue(self, name):
		self.values.append(name)

	def dump(self, file, namespace, index, nested):
		decl = "enum%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl))
		if showline: file.write("#line %d\n" % (self.line))
		name = self.fullname[2:].replace('::', '.')
		file.write("    static inline ref< ::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    {\n")
		file.write("        ::BugEngine::Logger::root();\n")
		file.write("        static minitl::ref< ::BugEngine::RTTI::ClassInfo> klass;\n")
		file.write("        if (klass)\n")
		file.write("            return klass;\n")
		if nested  or self.name.find('::') != -1:
			file.write("        ::BugEngine::be_typeid< %s >::klass();\n" % '::'.join(self.fullname.split('::')[:-1]))
		file.write("        klass = ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< void >::klass(), be_checked_numcast<u32>(sizeof(%s)), 0);\n" % (self.fullname[2:].replace('::', '.'), self.fullname))
		for tag in self.tags:
			file.write("        klass->tags.push_back(Value(%s));\n" % tag)
		file.write("        weak<RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		file.write("        inamespace name = inamespace(\"%s\");\n" % (name))
		file.write("        ns->add(name, Value(minitl::ref< const ::BugEngine::RTTI::ClassInfo >(klass)));\n")
		file.write("        return klass;\n");
		file.write("    }\n");
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		file.write("        klass->copyconstructor = &::BugEngine::RTTI::wrapCopy< %s >;\n" % (self.fullname))
		file.write("        klass->destructor = &::BugEngine::RTTI::wrapDestroy< %s >;\n" % (self.fullname))
		ns = '::'.join(self.fullname.split('::')[:-1])
		for enum in self.values:
			#if showline: file.write("        #line %d\n" % (line))
			file.write("        klass->add(BugEngine::istring(\"%s\"), Value(%s));\n" % (enum, ns+'::'+enum))
		file.write("    }\n")
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		file.write("        klass->tags.clear();\n")
		file.write("        klass->methods.clear();\n")
		file.write("        klass->properties.clear();\n")
		file.write("        klass->constructor.overloads.clear();\n")
		file.write("        klass->call.overloads.clear();\n")
		file.write("        klass->decls.clear();\n")
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1, nested)
		self.parent.classes += self.classes
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line, scope, value):
		Container.__init__(self, parent, name, line, scope)
		self.inherits = inherits or 'void'
		self.value = value
		self.metaclass = '::BugEngine::RTTI::ClassInfo'
		self.tags = []

	def dump(self, file, namespace, index, nested):
		decl = "class%s" % self.fullname.replace(':', '_')
		self.classes.append((self.fullname, namespace + '::s_' + decl))
		self.writeClass(file, decl, nested)
		self.buildProperties(file, decl)
		self.cleanProperties(file, decl)
		index = Container.dump(self, file, namespace, index+1, True)
		self.parent.classes += self.classes
		return index

	def writeClass(self, file, decl, nested):
		name = self.fullname[2:].replace('::', '.')
		file.write("    static inline minitl::ref< ::BugEngine::RTTI::ClassInfo> s_%sClass()\n" % decl)
		file.write("    {\n")
		file.write("        ::BugEngine::Logger::root();\n")
		file.write("        static minitl::ref< ::BugEngine::RTTI::ClassInfo> klass;\n")
		file.write("        if (klass)\n")
		file.write("            return klass;\n")
		file.write("        inamespace name = inamespace(\"%s\");\n" % (name))
		if nested or self.name.find('::') != -1:
			file.write("        ::BugEngine::be_typeid< %s >::klass();\n" % '::'.join(self.fullname.split('::')[:-1]))
		if self.inherits != 'void':
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent = ::BugEngine::be_typeid< %s >::klass();\n" % self.inherits)
		else:
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent;\n")
		if showline: file.write("#line %d\n" % (self.line))
		file.write("        if (klass)\n")
		file.write("            return klass;\n")
		file.write("        klass = minitl::ref< %s >::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent, be_checked_numcast<u32>(sizeof(%s)), be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1));\n" % (self.metaclass, self.fullname[2:].replace('::', '.'), self.fullname, self.inherits, self.fullname))
		if self.value:
			file.write("        klass->copyconstructor = &::BugEngine::RTTI::wrapCopy< %s >;\n" % (self.fullname))
			file.write("        klass->destructor = &::BugEngine::RTTI::wrapDestroy< %s >;\n" % (self.fullname))
		for tag in self.tags:
			file.write("        klass->tags.push_back(Value(%s));\n" % tag)
		file.write("        weak<RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		file.write("        ns->add(name, Value(minitl::ref< const ::BugEngine::RTTI::ClassInfo >(klass)));\n")
		file.write("        return klass;\n");
		file.write("    }\n")


	def buildProperties(self, file, decl):
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		file.write("        ::BugEngine::TypeInfo type = ::BugEngine::be_typeid< %s >::type();\n" % self.fullname)
		for type,name,attr,visibility,tags,line in self.members:
			if visibility == 'published':
				if showline: file.write("        #line %d\n" % (line))
				file.write("        klass->addProperty(\"%s\", ::BugEngine::RTTI::PropertyInfo(type, BugEngine::be_typeid< %s >::type(), be_checked_numcast<u32>((char*)(&((%s*)0)->%s)-(char*)0)));\n" % (name, type, self.fullname, name))
		self.buildMethods(file, decl)
		file.write("    }\n")

	def buildMethods(self, file, decl):
		methodindex = 0
		for name, overloads in self.methods.iteritems():
			if name == "?dtor":
				continue
			file.write("        {\n")
			if name == "?call":
				file.write("            ::BugEngine::RTTI::MethodInfo& mi = klass->call;\n")
			elif name == "?ctor":
				file.write("            ::BugEngine::RTTI::MethodInfo& mi = klass->constructor;\n")
			else:
				file.write("            ::BugEngine::RTTI::MethodInfo mi;\n")
			file.write("            mi.overloads.reserve(%d);\n" % len(overloads))
			for rtype, params, attrs, visibility, tags, line in overloads:
				if showline: file.write("            #line %d\n" % (line))
				paramtypes = ', '.join(ptype for ptype, pname in params)
				if paramtypes: paramtypes = ', '+paramtypes
				if name == '?call':
					method = "&%s::%s" % (self.fullname, "operator()")
				else:
					method = "&%s::%s" % (self.fullname, name)

				if name == '?ctor':
					if self.value:
						call = "&BugEngine::RTTI::callhelper< %s, %s%s >::construct" % (self.fullname, 'void', paramtypes)
					else:
						call = "&BugEngine::RTTI::callhelper< %s, %s%s >::constructPtr" % (self.fullname, 'void', paramtypes)
				elif 'const' in attrs:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::callConst< %s >" % (self.fullname, rtype, paramtypes, method)
				elif 'static' in attrs:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::callStatic< %s >" % (self.fullname, rtype, paramtypes, method)
				else:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::call< %s >" % (self.fullname, rtype, paramtypes, method)
				file.write("            if (BugEngine::RTTI::callhelper< %s, %s%s >::VarArg)\n" % (self.fullname, rtype, paramtypes))
				file.write("            {\n")
				file.write("                mi.overloads.push_back(::BugEngine::RTTI::OverloadInfo(::BugEngine::be_typeid< %s >::type(), %s, true));\n" % (rtype, call))
				file.write("            }\n")
				file.write("            else\n")
				file.write("            {\n")
				file.write("                mi.overloads.push_back(::BugEngine::RTTI::OverloadInfo(::BugEngine::be_typeid< %s >::type(), %s, false));\n" % (rtype, call))
				if 'static' not in attrs:
					if "const" in attrs:
						file.write("                mi.overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"this\", ::BugEngine::be_typeid< %s const* >::type()));\n" % (self.fullname))
					else:
						file.write("                mi.overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"this\", ::BugEngine::be_typeid< %s* >::type()));\n" % (self.fullname))
				for ptype, pname in params:
					file.write("                mi.overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"%s\", ::BugEngine::be_typeid< %s >::type()));\n" % (pname, ptype))
				file.write("            }\n")
				methodindex = methodindex + 1
			file.write("            klass->addMethod(\"%s\", mi);\n" % (name))
			file.write("        }\n")


	def cleanProperties(self, file, decl):
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo > klass = s_%sClass();\n" % decl)
		file.write("        klass->tags.clear();\n")
		file.write("        klass->methods.clear();\n")
		file.write("        klass->properties.clear();\n")
		file.write("        klass->constructor.overloads.clear();\n")
		file.write("        klass->call.overloads.clear();\n")
		file.write("        klass->decls.clear();\n")
		file.write("    }\n")

