import os

try:
	import hashlib
	make_md5 = hashlib.md5
except:
	import md5
	make_md5 = md5.new

showline = 0


def toOperator(name):
	if name.startswith("operator"):
		name = name[8:]
		if name == '+':
			return "?add"
		if name == '-':
			return "?sub"
		if name == '*':
			return "?mul"
		if name == '/':
			return "?div"
		if name == '%':
			return "?mod"
		if name == '&':
			return "?band"
		if name == '|':
			return "?bor"
		if name == '&&':
			return "?and"
		if name == '||':
			return "?or"
		if name == '<<':
			return "?shl"
		if name == '>>':
			return "?shr"
		if name == '!':
			return "?not"
		if name == '~':
			return "?bnot"
		if name == '==':
			return "?eq"
		if name == '<=':
			return "?leq"
		if name == '<':
			return "?less"
		if name == '>=':
			return "?seq"
		if name == '>':
			return "?sup"
		if name == '[]':
			return "?index"
		if name == '()':
			return "?call"
	return name

class Container:
	def __init__(self, name, line):
		self.name = name
		self.objects = []
		self.members = []
		self.methods = {}
		self.line = line
		self.classes = []
		self.hash = make_md5()

	def addObject(self, object):
		self.objects.append(object)

	def addMember(self, type, attr, name, tags, line):
		self.members.append((type, name, attr, tags, line))

	def addMethod(self, name, attr, rtype, params, tags, line):
		if name in self.methods:
			self.methods[name].append((rtype, params, attr, tags, line))
		else:
			self.methods[name] = [(rtype, params, attr, tags, line)]

	def dump(self, file, instances, useMethods, plugin, namespace, parent, fullname):
		classes = []
		for o in self.objects:
			classes += o.dump(file, instances, useMethods, plugin, namespace, parent, fullname)
		return classes

	def predecl(self, file, instances, useMethods, plugin, fullname):
		for o in self.objects:
			o.predecl(file, instances, useMethods, plugin, fullname)

	def writeTags(self, file, prefix, tags):
		tagname = "0"
		tagindex = 0
		for type,tag in tags:
			file.write("static %s s_%s_tag_value_%d = %s(%s);\n" % (type, prefix, tagindex, type, tag))
			file.write("static ::BugEngine::RTTI::Tag s_%s_tag_%d =\n" % (prefix, tagindex))
			file.write("    {\n")
			file.write("        {%s},\n" % tagname)
			file.write("        ::BugEngine::RTTI::Value(be_typeid< %s >::type(), (void*)&s_%s_tag_value_%d)\n" % (type, prefix, tagindex))
			file.write("    };\n")
			tagname = "&s_%s_tag_%d" % (prefix, tagindex)
			tagindex = tagindex + 1
		return tagname


class Root(Container):
	def __init__(self):
		Container.__init__(self, None, '')

	def dump(self, file, instances, useMethods, plugin, source, namespace = '', parent = '', fullname = ''):
		instances.write("#include    <rtti/stdafx.h>\n")
		instances.write("#include    <%s>\n" % source)
		instances.write("#include    <rtti/typeinfo.hh>\n")
		instances.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % source)
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
		file.write("#include    <rtti/tags/documentation.script.hh>\n")
		file.write("\n")

		if showline: file.write("#line %d \"%s\"\n" % (self.line, source.replace("\\", "\\\\")))

		file.write("namespace BugEngine\n{\n\n")
		instances.write("namespace BugEngine\n{\n\n")
		self.predecl(file, instances, useMethods, plugin, fullname)
		file.write("\n}\n\n")
		instances.write("\n}\n\n")

		classes = Container.dump(self, file, instances, useMethods, plugin, namespace, parent, fullname)
		instances.write("namespace BugEngine\n{\n\n")
		for classname, objname in classes:
			instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (classname, objname))
		instances.write("\n}\n\n")


class Typedef(Container):
	def __init__(self, name, line):
		Container.__init__(self, name, line)


class Namespace(Container):
	def __init__(self, name, line):
		Container.__init__(self, name, line)

	def dump(self, file, instances, useMethods, plugin, namespace, parent, fullname):
		fullname = fullname + '::' + self.name
		name = fullname.split('::')[1:]
		owner = "be_%s_Namespace"%plugin
		for ns in name:
			owner += "_%s" % ns
		owner += "()"

		if showline: file.write("#line %d\n" % self.line)
		file.write("namespace %s\n" % self.name)
		file.write("{\n")
		decl = "namespace%s" % fullname.replace(':', '_')
		self.buildProperties(file, decl, owner, fullname)
		self.buildMethods(file, decl, owner, fullname)
		classes = Container.dump(self, file, instances, useMethods, plugin, namespace + '::' + self.name, owner, fullname)
		file.write("}\n")
		return classes

	def predecl(self, file, instances, useMethods, plugin, fullname):
		fullname = fullname + '::' + self.name
		if self.name != 'BugEngine':
			file.write("using namespace %s;\n"%fullname)
			instances.write("namespace %s\n{\n"%self.name)
		file.write("raw< ::BugEngine::RTTI::Class > be_%s_Namespace%s();\n"%(plugin, "_".join(fullname.split('::'))))
		Container.predecl(self, file, instances, useMethods, plugin, fullname)
		if self.name != 'BugEngine':
			instances.write("}\n")

	def buildProperties(self, file, decl, parent, fullname):
		statics = "%s->objects.m_ptr" % parent
		for type,name,attr,tags,line in self.members[::-1]:
			if showline:
				file.write("#line %d\n" % (line))
			self.hash.update("property".encode())
			self.hash.update(type.encode())
			self.hash.update(name.encode())
			tagname = self.writeTags(file, decl+"_"+name, tags)
			file.write("static const ::BugEngine::RTTI::Class::ObjectInfo s_%s_%s =\n" % (decl, name))
			file.write("    {\n")
			file.write("        {%s},\n" % statics)
			file.write("        {%s},\n" % tagname)
			file.write("        \"%s\",\n" % name)
			file.write("        ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s::%s))\n" % (fullname, name))
			file.write("    };\n")
			statics = "&s_%s_%s" % (decl, name)
		if self.members:
			file.write("const ::BugEngine::RTTI::Class::ObjectInfo* s_%s_%s_obj_ptr = ( %s->objects.set(%s) );\n" % (decl, self.hash.hexdigest(), parent, statics))

	def buildMethods(self, file, decl, parent, fullname):
		method = "%s->methods.m_ptr" % parent
		for name, overloads in self.methods.items():
			overload = "0"
			overloadindex = 0

			prettyname = toOperator(name)
			prettyname = prettyname.replace("?", "_")
			if showline:
				file.write("        #line %d\n" % (line))
			self.hash.update("method".encode())
			self.hash.update(name.encode())

			for rtype, params, attrs, tags, line in overloads:
				self.hash.update("overload".encode())
				if "const" in attrs:
					self.hash.update("const".encode())
				if "static" in attrs:
					self.hash.update("static".encode())
				paramindex = 0
				param = "0"
				if showline: file.write("#line %d\n" % (line))
				method_tagname = self.writeTags(file, decl+"_"+prettyname, tags)
				for ptype, pname, tags in params[::-1]:
					self.hash.update("param".encode())
					self.hash.update(ptype.encode())
					self.hash.update(pname.encode())
					param_tagname = self.writeTags(file, decl+"_"+prettyname+"_"+pname, tags)
					file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        {%s},\n" % param_tagname)
					file.write("        {%s},\n" % param)
					file.write("        \"%s\",\n" % pname)
					file.write("        ::BugEngine::be_typeid< %s >::type()\n" % ptype)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)
					paramindex = paramindex + 1
				paramtypes = ', '.join(ptype for ptype, pname, tags in params)
				ptr = "%s (*) (%s)" % (rtype, paramtypes)
				methodptr = "BE_SELECTOVERLOAD(%s)&%s::%s" % (ptr, fullname, name)
				if paramtypes: paramtypes = ', '+paramtypes
				if rtype != 'void':
					helper = "BugEngine::RTTI::functionhelper< %s, %s%s >" % ("::BugEngine::RTTI::Class", rtype, paramtypes)
				else:
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % ("::BugEngine::RTTI::Class", paramtypes)
				callptr = "&%s::callStatic< %s >" % (helper, methodptr)

				file.write("static const ::BugEngine::RTTI::Method::Overload s_%s_%s_%d =\n" % (decl, prettyname, overloadindex))
				file.write("    {\n")
				file.write("        {%s},\n" % method_tagname)
				file.write("        {%s},\n" % overload)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % rtype)
				file.write("        {%s},\n" % param)
				file.write("        %s::VarArg,\n" % helper)
				file.write("        %s\n" % callptr)
				file.write("    };\n")
				overload = "&s_%s_%s_%d" % (decl, prettyname, overloadindex)
				overloadindex = overloadindex + 1

			file.write("static const ::BugEngine::RTTI::Method s_method_%s_%s =\n" % (decl, prettyname))
			file.write("    {\n")
			file.write("        \"%s\",\n" % name)
			file.write("        {&s_method_%s_%s},\n" % (decl, prettyname))
			file.write("        {%s},\n" % method)
			file.write("        {%s}\n" % overload)
			file.write("    };\n")
			method = "&s_method_%s_%s" % (decl, prettyname)
		if self.methods:
			file.write("static const ::BugEngine::RTTI::Method* s_%s_%s_method_ptr = { %s->methods.set(%s) };\n" % (decl, self.hash.hexdigest(), parent, method))


class Enum(Container):
	def __init__(self, name, line):
		Container.__init__(self, name, line)
		self.values = []
		self.tags = []

	def addEnumValue(self, name):
		self.values.append(name)

	def predecl(self, file, instances, useMethods, plugin, fullname):
		fullname = fullname + '::' + self.name
		decl = "enum%s" % fullname.replace(':', '_')
		if useMethods:
			instances.write("static const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))
		Container.predecl(self, file, instances, useMethods, plugin, fullname)

	def dump(self, file, instances, useMethods, plugin, namespace, parent, fullname):
		fullname = fullname+'::'+self.name
		name = fullname[2:].replace('::', '.')
		decl = "enum%s" % fullname.replace(':', '_')
		tagname = self.writeTags(file, decl, self.tags)
		if useMethods:
			file.write("static const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		hash = self.hash.hexdigest()
		file.write("::BugEngine::RTTI::Class s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (name))
		file.write("        be_typeid< void >::klass(),\n")
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % fullname)
		file.write("        0,\n")
		file.write("        {%s},\n" % (tagname))
		file.write("        {0},\n")
		file.write("        {0},\n")
		file.write("        {0},\n")
		file.write("        {0},\n")
		file.write("        {0},\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >,\n" % fullname)
		file.write("        {{ 0x%s, 0x%s, 0x%s, 0x%s }}\n" % (hash[0:8], hash[8:16], hash[16:24], hash[24:32]))
		file.write("    };\n")

		if useMethods:
			file.write("return s_%s;\n}\n" % decl)
		file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj = { %s->objects, {%s}, \"%s\", ::BugEngine::RTTI::Value(&s_%s) };\n" % (decl, parent, tagname, self.name, decl))
		file.write("const ::BugEngine::RTTI::Class::ObjectInfo* s_%s_obj_ptr = ( %s->objects.set(&s_%s_obj) );\n" % (decl, parent, decl))

		if useMethods:
			return [(fullname, "%s::s_%sFun()" % (namespace, decl))]
		else:
			return [(fullname, "%s::s_%s" % (namespace, decl))]


class Class(Container):
	def __init__(self, name, inherits, line, value):
		Container.__init__(self, name, line)
		self.inherits = inherits or 'void'
		self.value = value
		self.tags = []

	def predecl(self, file, instances, useMethods, plugin, fullname):
		fullname = fullname + '::' + self.name
		decl = "class%s" % fullname.replace(':', '_')
		if useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))
		Container.predecl(self, file, instances, useMethods, plugin, fullname)

	def dump(self, file, instances, useMethods, plugin, namespace, parent, fullname):
		fullname = fullname + '::' + self.name
		file.write("//----------------------------------------------------------------------\n")
		file.write("// %s\n" % fullname)
		self.hash.update(fullname.encode())
		decl = "class%s" % fullname.replace(':', '_')
		if useMethods:
			varname = "%s::s_%sFun()" % (namespace, decl)
		else:
			varname = "%s::s_%s" % (namespace, decl)
		if useMethods:
			file.write("static const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		properties, statics = self.buildProperties(file, decl, fullname)
		methods, constructor, call = self.buildMethods(file, decl, fullname);
		self.writeClass(file, decl, parent, fullname, properties, methods, statics, constructor, call)
		if useMethods:
			file.write("return s_%s;\n}\n" % decl)
		classes = Container.dump(self, file, instances, useMethods, plugin, namespace, "(&%s)"%varname, fullname)
		classes.append((fullname, varname))
		file.write("//----------------------------------------------------------------------\n\n")
		return classes

	def writeClass(self, file, decl, parent, fullname, properties, methods, objects, constructor, call):
		name = fullname[2:].replace('::', '.')
		tagname = self.writeTags(file, decl, self.tags)
		hash = self.hash.hexdigest()
		file.write("::BugEngine::RTTI::Class s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (name))
		file.write("        be_typeid< %s >::klass(),\n" % (self.inherits))
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % fullname)
		file.write("        be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1),\n" % (self.inherits, fullname))
		file.write("        {%s},\n" % (tagname))
		file.write("        {%s},\n" % (properties))
		file.write("        {%s},\n" % (methods))
		file.write("        {%s},\n" % (objects))
		file.write("        {%s},\n" % (constructor))
		file.write("        {%s},\n" % (constructor))
		if self.value:
			file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
			file.write("        &::BugEngine::RTTI::wrapDestroy< %s >,\n" % fullname)
		else:
			file.write("        0,\n")
			file.write("        0,\n")
		file.write("        {{ 0x%s, 0x%s, 0x%s, 0x%s }}\n" % (hash[0:8], hash[8:16], hash[16:24], hash[24:32]))
		file.write("    };\n")
		file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj = { %s->objects, {%s}, \"%s\", ::BugEngine::RTTI::Value(&s_%s) };\n" % (decl, parent, tagname, self.name, decl))
		file.write("const ::BugEngine::RTTI::Class::ObjectInfo* s_%s_obj_ptr = ( %s->objects.set(&s_%s_obj) );\n" % (decl, parent, decl))

	def buildProperties(self, file, decl, fullname):
		statics = "be_typeid< %s >::klass()->objects.operator->()"%self.inherits
		prop = "0"
		for type,name,attr,tags,line in self.members[::-1]:
			if showline:
				file.write("#line %d\n" % (line))
			self.hash.update("property".encode())
			self.hash.update(type.encode())
			self.hash.update(name.encode())
			tagname = self.writeTags(file, decl+"_"+name, tags)
			if 'static' in attr:
				file.write("static const ::BugEngine::RTTI::Class::ObjectInfo s_%s_%s =\n" % (decl, name))
				file.write("    {\n")
				file.write("        {%s},\n" % statics)
				file.write("        {%s},\n" % tagname)
				file.write("        \"%s\",\n" % name)
				file.write("        ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s::%s))\n" % (fullname, name))
				file.write("    };\n")
				statics = "&s_%s_%s" % (decl, name)
			else:
				file.write("static const ::BugEngine::RTTI::Property s_%s_%s =\n" % (decl, name))
				file.write("    {\n")
				file.write("        {%s},\n" % tagname)
				file.write("        {%s},\n" % prop)
				file.write("        \"%s\",\n" % name)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % fullname)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % type)
				file.write("        &::BugEngine::RTTI::PropertyHelper<%s, %s, &%s::%s>::get\n" % (type, fullname, fullname, name))
				file.write("    };\n")
				prop = "&s_%s_%s" % (decl, name)
		return prop, statics

	def buildMethods(self, file, decl, fullname):
		method = "0"
		constructor = "0"
		call = "0"
		for name, overloads in self.methods.items():
			overload = "0"
			overloadindex = 0

			prettyname = toOperator(name)
			prettyname = prettyname.replace("?", "_")
			if name == "?dtor":
				continue
			if showline:
				file.write("        #line %d\n" % (line))
			self.hash.update("method".encode())
			self.hash.update(name.encode())

			for rtype, params, attrs, tags, line in overloads:
				self.hash.update("overload".encode())
				if "const" in attrs:
					self.hash.update("const".encode())
				if "static" in attrs:
					self.hash.update("static".encode())
				paramindex = 0
				param = "0"
				if showline: file.write("#line %d\n" % (line))
				method_tagname = self.writeTags(file, decl+"_"+prettyname+"_%d"%overloadindex, tags)
				for ptype, pname, tags in params[::-1]:
					self.hash.update("param".encode())
					self.hash.update(ptype.encode())
					self.hash.update(pname.encode())
					param_tagname = self.writeTags(file, decl+"_"+prettyname+("_%d_"%overloadindex)+pname, tags)
					file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        {%s},\n" % param_tagname)
					file.write("        {%s},\n" % param)
					file.write("        \"%s\",\n" % pname)
					file.write("        ::BugEngine::be_typeid< %s >::type()\n" % ptype)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)
					paramindex = paramindex + 1
				if 'static' not in attrs:
					file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        {0},\n")
					file.write("        {%s},\n" % param)
					file.write("        \"this\",\n")
					if "const" in attrs:
						file.write("        ::BugEngine::be_typeid< %s const* >::type()\n" % fullname)
					else:
						file.write("        ::BugEngine::be_typeid< %s * >::type()\n" % fullname)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)

				paramtypes = ', '.join(ptype for ptype, pname, tags in params)
				if 'static' in attrs:
					ptr = "%s (*) (%s)" % (rtype, paramtypes)
				elif 'const' in attrs:
					ptr = "%s (%s::*) (%s) const" % (rtype, fullname, paramtypes)
				else:
					ptr = "%s (%s::*) (%s)" % (rtype, fullname, paramtypes)
				methodptr = "BE_SELECTOVERLOAD(%s)&%s::%s" % (ptr, fullname, name)
				if paramtypes: paramtypes = ', '+paramtypes
				if name == '?ctor':
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (fullname, paramtypes)
				elif rtype != 'void':
					helper = "BugEngine::RTTI::functionhelper< %s, %s%s >" % (fullname, rtype, paramtypes)
				else:
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (fullname, paramtypes)
				if name == '?ctor':
					if self.value:
						callptr = "&%s::construct" % helper
						rtype = fullname
					else:
						callptr = "&%s::constructPtr" % helper
						rtype = "ref< %s >" % fullname
				elif 'const' in attrs:
						callptr = "&%s::callConst< %s >" % (helper, methodptr)
				elif 'static' in attrs:
						callptr = "&%s::callStatic< %s >" % (helper, methodptr)
				else:
					callptr = "&%s::call< %s >" % (helper, methodptr)


				file.write("static const ::BugEngine::RTTI::Method::Overload s_%s_%s_%d =\n" % (decl, prettyname, overloadindex))
				file.write("    {\n")
				file.write("        {%s},\n" % method_tagname)
				file.write("        {%s},\n" % overload)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % rtype)
				file.write("        {%s},\n" % param)
				file.write("        %s::VarArg,\n" % helper)
				file.write("        %s\n" % callptr)
				file.write("    };\n")
				overload = "&s_%s_%s_%d" % (decl, prettyname, overloadindex)
				overloadindex = overloadindex + 1

			file.write("static const ::BugEngine::RTTI::Method s_method_%s_%s =\n" % (decl, prettyname))
			file.write("    {\n")
			file.write("        \"%s\",\n" % name)
			file.write("        {&s_method_%s_%s},\n" % (decl, prettyname))
			file.write("        {%s},\n" % method)
			file.write("        {%s}\n" % overload)
			file.write("    };\n")
			if name == "?ctor":
				constructor = "&s_method_%s_%s" % (decl, prettyname)
			elif name == "?call":
				call = "&s_method_%s_%s" % (decl, prettyname)
			method = "&s_method_%s_%s" % (decl, prettyname)

		return method, constructor, call

