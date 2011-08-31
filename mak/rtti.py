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
		self.scope = scope
		self.visibility = 'published'

	def addObject(self, object):
		self.objects.append(object)

	def addMember(self, type, attr, name, tags, line):
		self.members.append((type, name, attr, self.visibility, tags, line))

	def addMethod(self, name, attr, rtype, params, tags, line):
		if self.visibility == 'published':
			if name in self.methods:
				self.methods[name].append((rtype, params, attr, self.visibility, tags, line))
			else:
				self.methods[name] = [(rtype, params, attr, self.visibility, tags, line)]

	def dump(self, file, namespace, index, nested):
		for o in self.objects:
			if o.scope == 'published':
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
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/taginfo.script.hh>\n")
		file.write("#include    <rtti/engine/helper/get.hh>\n")
		file.write("#include    <rtti/engine/helper/set.hh>\n")
		file.write("#include    <rtti/engine/helper/method.hh>\n")
		file.write("\n")
		file.write("namespace BugEngine {\n\n")

		if showline: file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))
		
		Container.dump(self, file, namespace, index, False)

		file.write("}\n")
		return index

class Typedef(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'published')

	def dump(self, file, namespace, index, nested):
		if showline: file.write("#line %d\n" % self.line)
		if self.name != 'BugEngine':
			file.write("using namespace %s;\n" % self.fullname)
		index = Container.dump(self, file, namespace + '::' + self.name, index, False)
		return index

class Enum(Container):
	def __init__(self, parent, name, line, scope):
		Container.__init__(self, parent, name, line, scope)
		self.values = []
		self.tags = []

	def addEnumValue(self, name):
		self.values.append(name)

	def dump(self, file, namespace, index, nested):
		name = self.fullname[2:].replace('::', '.')
		decl = "enum%s" % self.fullname.replace(':', '_')
		tagname = "0"
		tagindex = 0
		for type,tag in self.tags:
			file.write("static %s s_%s_tag_value_%d = %s(%s);\n" % (type, decl, tagindex, type, tag))
			file.write("static RTTI::TagInfo s_%s_tag_%d =\n" % (decl, tagindex))
			file.write("    {\n")
			file.write("        %s,\n" % tagname)
			file.write("        Value(be_typeid< %s >::type(), (void*)&s_%s_tag_value_%d)\n" % (type, decl, tagindex))
			file.write("    };\n")
			tagname = "&s_%s_tag_%d" % (decl, tagindex)
			tagindex = tagindex + 1

		file.write("static const RTTI::ClassInfo s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (name))
		file.write("        be_typeid< void >::klass(),\n")
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % self.fullname)
		file.write("        0,\n")
		file.write("        %s,\n" % (tagname))
		file.write("        0,\n")
		file.write("        0,\n")
		file.write("        0,\n")
		file.write("        0,\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % self.fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % self.fullname)
		file.write("    };\n")
		file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass() { return &s_%s; }\n" % (self.fullname, decl))
		return index

class Class(Container):
	def __init__(self, parent, name, inherits, line, scope, value):
		Container.__init__(self, parent, name, line, scope)
		self.inherits = inherits or 'void'
		self.value = value
		self.tags = []

	def dump(self, file, namespace, index, nested):
		file.write("//----------------------------------------------------------------------\n")
		file.write("// %s\n" % self.fullname)
		index = Container.dump(self, file, namespace, index+1, True)
		decl = "class%s" % self.fullname.replace(':', '_')
		properties = self.buildProperties(file, decl)
		methods, constructor, call = self.buildMethods(file, decl);
		self.writeClass(file, decl, nested, properties, methods, constructor, call)
		file.write("//----------------------------------------------------------------------\n\n")
		return index

	def writeClass(self, file, decl, nested, properties, methods, constructor, call):
		name = self.fullname[2:].replace('::', '.')
		tagname = "0"
		tagindex = 0
		for type,tag in self.tags:
			file.write("static %s s_%s_tag_value_%d = %s(%s);\n" % (type, decl, tagindex, type, tag))
			file.write("static RTTI::TagInfo s_%s_tag_%d =\n" % (decl, tagindex))
			file.write("    {\n")
			file.write("        %s,\n" % tagname)
			file.write("        Value(be_typeid< %s >::type(), (void*)&s_%s_tag_value_%d)\n" % (type, decl, tagindex))
			file.write("    };\n")
			tagname = "&s_%s_tag_%d" % (decl, tagindex)
			tagindex = tagindex + 1

		file.write("static const RTTI::ClassInfo s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (name))
		file.write("        be_typeid< %s >::klass(),\n" % (self.inherits))
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % self.fullname)
		file.write("        be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1),\n" % (self.inherits, self.fullname))
		file.write("        %s,\n" % (tagname))
		file.write("        %s,\n" % (properties))
		file.write("        %s,\n" % (methods))
		file.write("        %s,\n" % (constructor))
		file.write("        %s,\n" % (call))
		if self.value:
			file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % self.fullname)
			file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % self.fullname)
		else:
			file.write("        0,\n")
			file.write("        0\n")
		file.write("    };\n")
		file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass() { return &s_%s; }\n" % (self.fullname, decl))

	def buildProperties(self, file, decl):
		prop = "0"
		for type,name,attr,visibility,tags,line in self.members[::-1]:
			if visibility == 'published':
				if showline:
					file.write("#line %d\n" % (line))
				file.write("static const ::BugEngine::RTTI::PropertyInfo s_%s_%s =\n" % (decl, name))
				file.write("    {\n")
				file.write("        %s,\n" % prop)
				file.write("        BugEngine::be_typeid< %s >::type(),\n" % self.fullname)
				file.write("        BugEngine::be_typeid< %s >::type(),\n" % type)
				file.write("        be_checked_numcast<u32>((char*)(&((%s*)0)->%s)-(char*)0)\n" % (self.fullname, name))
				file.write("    };\n")
				prop = "&s_%s_%s" % (decl, name)
		return prop

	def buildMethods(self, file, decl):
		method = "0"
		constructor = "0"
		call = "0"
		for name, overloads in self.methods.items():
			overload = "0"
			overloadindex = 0

			prettyname = name.replace("?", "_")
			if name == "?dtor":
				continue
			if showline:
				file.write("        #line %d\n" % (line))

			for rtype, params, attrs, visibility, tags, line in overloads:
				paramindex = 0
				param = "0"
				if showline: file.write("#line %d\n" % (line))
				for ptype, pname in params[::-1]:
					file.write("static const ::BugEngine::RTTI::MethodInfo::OverloadInfo::ParamInfo s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        %s,\n" % param)
					file.write("        \"%s\",\n" % pname)
					file.write("        ::BugEngine::be_typeid< %s >::type()\n" % ptype)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)
					paramindex = paramindex + 1
				if 'static' not in attrs:
					file.write("static const ::BugEngine::RTTI::MethodInfo::OverloadInfo::ParamInfo s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        %s,\n" % param)
					file.write("        \"this\",\n")
					if "const" in attrs:
						file.write("        ::BugEngine::be_typeid< %s const* >::type()\n" % self.fullname)
					else:
						file.write("        ::BugEngine::be_typeid< %s * >::type()\n" % self.fullname)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)

				paramtypes = ', '.join(ptype for ptype, pname in params)
				if 'static' in attrs:
					ptr = "%s (*) (%s)" % (rtype, paramtypes)
				elif 'const' in attrs:
					ptr = "%s (%s::*) (%s) const" % (rtype, self.fullname, paramtypes)
				else:
					ptr = "%s (%s::*) (%s)" % (rtype, self.fullname, paramtypes)
				if name == '?call':
					methodptr = "BE_SELECTOVERLOAD(%s)&%s::%s" % (ptr, self.fullname, "operator()")
				else:
					methodptr = "BE_SELECTOVERLOAD(%s)&%s::%s" % (ptr, self.fullname, name)
				if paramtypes: paramtypes = ', '+paramtypes
				if name == '?ctor':
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (self.fullname, paramtypes)
				elif rtype != 'void':
					helper = "BugEngine::RTTI::functionhelper< %s, %s%s >" % (self.fullname, rtype, paramtypes)
				else:
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (self.fullname, paramtypes)
				if name == '?ctor':
					if self.value:
						callptr = "&%s::construct" % helper
					else:
						callptr = "&%s::constructPtr" % helper
				elif 'const' in attrs:
						callptr = "&%s::callConst< %s >" % (helper, methodptr)
				elif 'static' in attrs:
						callptr = "&%s::callStatic< %s >" % (helper, methodptr)
				else:
					callptr = "&%s::call< %s >" % (helper, methodptr)


				file.write("static const ::BugEngine::RTTI::MethodInfo::OverloadInfo s_%s_%s_%d =\n" % (decl, prettyname, overloadindex))
				file.write("    {\n")
				file.write("        %s,\n" % overload)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % rtype)
				file.write("        %s,\n" % param)
				file.write("        %s::VarArg,\n" % helper)
				file.write("        %s\n" % callptr)
				file.write("    };\n")
				overload = "&s_%s_%s_%d" % (decl, prettyname, overloadindex)
				overloadindex = overloadindex + 1

			file.write("static const ::BugEngine::RTTI::MethodInfo s_%s_%s =\n" % (decl, prettyname))
			file.write("    {\n")
			file.write("        %s,\n" % method)
			file.write("        \"%s\",\n" % name)
			file.write("        %s\n" % overload)
			file.write("    };\n")
			if name == "?ctor":
				constructor = "&s_%s_%s" % (decl, prettyname)
			elif name == "?call":
				call = "&s_%s_%s" % (decl, prettyname)
			else:
				method = "&s_%s_%s" % (decl, prettyname)

		return method, constructor, call

		"""	file.write("static const ::BugEngine::RTTI::MethodInfo s_%s_%s =\n" % (decl, name))
				paramtypes = ', '.join(ptype for ptype, pname in params)

				if name == '?ctor':
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (self.fullname, paramtypes)
				elif rtype != 'void':
					helper = "BugEngine::RTTI::functionhelper< %s, %s%s >" % (self.fullname, rtype, paramtypes)
				else:
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (self.fullname, paramtypes)
				if name == '?ctor':
					if self.value:
						call = "&%s::construct" % helper
					else:
						call = "&%s::constructPtr" % helper
				elif 'const' in attrs:
						call = "&%s::callConst< %s >" % (helper, method)
				elif 'static' in attrs:
						call = "&%s::callStatic< %s >" % (helper, method)
				else:
					call = "&%s::call< %s >" % (helper, method)

				file.write("            if (%s::VarArg)\n" % (helper))
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
		return method, constructor, call"""
