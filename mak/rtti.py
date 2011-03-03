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

	def addMember(self, type, attr, name, line):
		self.members.append((type, name, attr, self.visibility, line))

	def addMethod(self, name, attr, rtype, params, line):
		if self.visibility == 'published':
			if self.methods.has_key(name):
				self.methods[name].append((rtype, params, attr, self.visibility, line))
			else:
				self.methods[name] = [(rtype, params, attr, self.visibility, line)]

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
		file.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/namespace.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/enuminfo.script.hh>\n")
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
			file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
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
		file.write("        if(klass)\n")
		file.write("            return klass;\n")
		if nested:
			file.write("        ::BugEngine::be_typeid< %s >::klass();\n" % '::'.join(self.fullname.split('::')[:-1]))
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< ::BugEngine::RTTI::ClassInfo>::klass());\n" % (name+'.metaclass'))
		file.write("        klass = ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< void >::klass(), metaclass, be_checked_numcast<u32>(sizeof(%s)), 0);\n" % (self.fullname[2:].replace('::', '.'), self.fullname))
		file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		file.write("        inamespace name = inamespace(\"%s\");\n" % (name))
		file.write("        ns->add(name, Value(minitl::ref< const ::BugEngine::RTTI::ClassInfo >(klass), klass->metaclass));\n")
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
			getter = '&::BugEngine::RTTI::staticgetvalue< %s, %s >' % (self.fullname, ns+'::'+enum)
			file.write("        klass->metaclass->addProperty(\"%s\", minitl::ref< ::BugEngine::RTTI::PropertyInfo >::create(::BugEngine::rttiArena(), BugEngine::be_typeid< %s >::type(), %s));\n" % (enum, self.fullname, getter))
		file.write("    }\n")
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		for enum in self.values:
			file.write("        klass->metaclass->removeProperty(\"%s\");\n" % (enum))
		file.write("    }\n")
		index = Container.dump(self, file, namespace, index+1, nested)
		self.parent.classes += self.classes
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line, scope, value):
		Container.__init__(self, parent, name, line, scope)
		self.inherits = inherits or 'void'
		self.value = value

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
		file.write("        if(klass)\n")
		file.write("            return klass;\n")
		if nested:
			file.write("        ::BugEngine::be_typeid< %s >::klass();\n" % '::'.join(self.fullname.split('::')[:-1]))
		if self.inherits != 'void':
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent = ::BugEngine::be_typeid< %s >::klass();\n" % self.inherits)
			file.write("        if(klass)\n")
			file.write("            return klass;\n")
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent->metaclass);\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		else:
			file.write("        minitl::ref<const ::BugEngine::RTTI::ClassInfo> parent;\n")
			file.write("        minitl::ref< ::BugEngine::RTTI::ClassInfo> metaclass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), ::BugEngine::be_typeid< ::BugEngine::RTTI::ClassInfo>::klass());\n" % (self.fullname[2:].replace('::', '.')+'.metaclass'))
		if showline: file.write("#line %d\n" % (self.line))
		file.write("        klass = minitl::ref< ::BugEngine::RTTI::ClassInfo>::create(::BugEngine::rttiArena(), ::BugEngine::inamespace(\"%s\"), parent, metaclass, be_checked_numcast<u32>(sizeof(%s)), be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1));\n" % (self.fullname[2:].replace('::', '.'), self.fullname, self.inherits, self.fullname))
		if self.value:
			file.write("        klass->copyconstructor = &::BugEngine::RTTI::wrapCopy< %s >;\n" % (self.fullname))
			file.write("        klass->destructor = &::BugEngine::RTTI::wrapDestroy< %s >;\n" % (self.fullname))
		file.write("        weak<const RTTI::Namespace> ns = RTTI::Namespace::rttiRoot();\n")
		file.write("        inamespace name = inamespace(\"%s\");\n" % (name))
		file.write("        ns->add(name, Value(minitl::ref< const ::BugEngine::RTTI::ClassInfo >(klass), klass->metaclass));\n")
		file.write("        return klass;\n");
		file.write("    }\n")


	def buildProperties(self, file, decl):
		for name, overloads in self.methods.iteritems():
			if name[0] == "?":
				continue
			file.write("    ref< ::BugEngine::RTTI::MethodInfo const > s_%s_%s;\n" % (decl, name))
		file.write("    static inline void s_%sRegisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo> klass = s_%sClass();\n" % decl)
		for type,name,attr,visibility,line in self.members:
			if visibility == 'published':
				if showline: file.write("        #line %d\n" % (line))
				getter = '&::BugEngine::RTTI::get< %s, %s, &%s::%s >' % (type, self.fullname, self.fullname, name)
				if attr.find('const') == -1:
					setter = ', &::BugEngine::RTTI::set< %s, %s, &%s::%s >' % (type, self.fullname, self.fullname, name)
				else:
					setter = ''
				file.write("        klass->addProperty(\"%s\", minitl::ref< ::BugEngine::RTTI::PropertyInfo >::create(::BugEngine::rttiArena(), BugEngine::be_typeid< %s >::type(), %s %s));\n" % (name, type, getter, setter))
		self.buildMethods(file, decl)


	def buildMethods(self, file, decl):
		methodindex = 0
		for name, overloads in self.methods.iteritems():
			if name == "?dtor":
				continue
			file.write("        {\n")
			file.write("            ref< ::BugEngine::RTTI::MethodInfo> mi = minitl::ref< ::BugEngine::RTTI::MethodInfo >::create(::BugEngine::rttiArena());\n")
			file.write("            mi->overloads.reserve(%d);\n" % len(overloads))
			for rtype, params, attrs, visibility, line in overloads:
				if showline: file.write("            #line %d\n" % (line))
				paramtypes = ', '.join(ptype for ptype, pname in params)
				if name == '?call':
					method = "&%s::%s" % (self.fullname, "operator()")
				elif name == '?ctor':
					rtype = 'BugEngine::Value'
					if self.value:
						method = "&::BugEngine::RTTI::ClassInfo::construct< weak<const ::BugEngine::RTTI::ClassInfo>, %s >" % paramtypes
					else:
						method = "&::BugEngine::RTTI::ClassInfo::constructPtr< %s >" % paramtypes
				else:
					method = "&%s::%s" % (self.fullname, name)

				if paramtypes: paramtypes = ', '+paramtypes
				if name == '?ctor':
					membername = '::BugEngine::RTTI::ClassInfo'
					params = [('weak<const ::BugEngine::RTTI::ClassInfo>', '')] + params
					paramtypes = ', weak<const ::BugEngine::RTTI::ClassInfo>' + paramtypes
				else:
					membername = self.fullname
				if 'const' in attrs:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::callConst< %s >" % (membername, rtype, paramtypes, method)
				elif 'static' in attrs:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::callStatic< %s >" % (membername, rtype, paramtypes, method)
				else:
					call = "&BugEngine::RTTI::callhelper< %s, %s%s >::call< %s >" % (membername, rtype, paramtypes, method)
				file.write("            if (BugEngine::RTTI::callhelper< %s, %s%s >::VarArg)\n" % (self.fullname, rtype, paramtypes))
				file.write("            {\n")
				file.write("                mi->vararg.call = %s;\n" % call)
				file.write("            }\n")
				file.write("            else\n")
				file.write("            {\n")
				file.write("                mi->overloads.push_back(::BugEngine::RTTI::OverloadInfo(::BugEngine::be_typeid< %s >::type(), %s));\n" % (rtype, call))
				if 'static' not in attrs:
					if "const" in attrs:
						file.write("                mi->overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"this\", ::BugEngine::be_typeid< %s const* >::type()));\n" % (membername))
					else:
						file.write("                mi->overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"this\", ::BugEngine::be_typeid< %s* >::type()));\n" % (membername))
				for ptype, pname in params:
					file.write("                mi->overloads.back().params.push_back(::BugEngine::RTTI::ParamInfo(\"%s\", ::BugEngine::be_typeid< %s >::type()));\n" % (pname, ptype))
				file.write("            }\n")
				methodindex = methodindex + 1
			if name == '?call':
				file.write("            klass->callOperator = mi;\n")
			elif name == '?ctor':
				file.write("            klass->metaclass->callOperator = mi;\n")
			else:
				methodname = "s_%s_%s" % (decl, name)
				file.write("            %s = mi;\n" % methodname)
				getter = '&::BugEngine::RTTI::staticgetobject< ref<const ::BugEngine::RTTI::MethodInfo>, &%s >' % methodname
				file.write("            klass->addProperty(\"%s\", minitl::ref<const ::BugEngine::RTTI::PropertyInfo >::create(::BugEngine::rttiArena(), BugEngine::be_typeid< ref<const ::BugEngine::RTTI::MethodInfo> >::type(), %s));\n" % (name, getter))
			file.write("        }\n")
		file.write("    }\n")


	def cleanProperties(self, file, decl):
		file.write("    static inline void s_%sUnregisterProperties()\n" % decl)
		file.write("    {\n")
		file.write("        minitl::weak< ::BugEngine::RTTI::ClassInfo > klass = s_%sClass();\n" % decl)
		file.write("        klass->metaclass->callOperator = ref<const ::BugEngine::RTTI::MethodInfo>();\n")
		file.write("        klass->callOperator = ref<const ::BugEngine::RTTI::MethodInfo>();\n")
		for type,name,attr,visibility,line in self.members:
			if visibility == 'published':
				if showline: file.write("        #line %d\n" % (line))
				file.write("        klass->removeProperty(\"%s\");\n" % (name))
		for name, overloads in self.methods.iteritems():
			if name[0] == "?":
				continue
			file.write("        klass->removeProperty(\"%s\");\n" % (name))
			file.write("        s_%s_%s = ref<const ::BugEngine::RTTI::MethodInfo>();\n" % (decl, name))
		file.write("    }\n")

