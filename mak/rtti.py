import os


try:
	import hashlib
	make_md5 = hashlib.md5
except:
	import md5
	make_md5 = md5.new

showline = 0

class Container:
	def __init__(self, parent, name, line, scope):
		self.parent = parent
		self.name = name
		if parent:
			self.fullname = parent.fullname
			self.useMethods = parent.useMethods
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
		self.classes = []
		self.hash = make_md5()

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

	def dump(self, file, namespace, nested):
		classes = []
		for o in self.objects:
			if o.scope == 'published':
				classes += o.dump(file, namespace, nested)
		return classes

	def predecl(self, file):
		for o in self.objects:
			o.predecl(file)

	def writeTags(self, file, prefix, tags):
		tagname = "0"
		tagindex = 0
		for type,tag in tags:
			file.write("static %s s_%s_tag_value_%d = %s(%s);\n" % (type, prefix, tagindex, type, tag))
			file.write("static ::BugEngine::RTTI::TagInfo s_%s_tag_%d =\n" % (prefix, tagindex))
			file.write("    {\n")
			file.write("        %s,\n" % tagname)
			file.write("        ::BugEngine::Value(be_typeid< %s >::type(), (void*)&s_%s_tag_value_%d)\n" % (type, prefix, tagindex))
			file.write("    };\n")
			tagname = "&s_%s_tag_%d" % (prefix, tagindex)
			tagindex = tagindex + 1
		return tagname


class Root(Container):
	def __init__(self, source, useMethods):
		Container.__init__(self, None, '', 1, 'public')
		self.source = source
		self.useMethods = useMethods

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

		if showline: file.write("#line %d \"%s\"\n" % (self.line, self.source.replace("\\", "\\\\")))

		file.write("namespace BugEngine\n{\n\n")
		self.predecl(file)
		file.write("\n}\n\n")

		prettysource = self.source.replace('/', '_').replace('\\', '_').replace('.', '_').replace(':', '_')
		classes = Container.dump(self, file, namespace, False)
		file.write("namespace BugEngine\n{\n\n")
		for classname, objname, nested in classes:
			if not nested:
				use = "be_forceuse(%s_obj_ptr); "%objname
			else:
				use = ""
			file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass() { %sreturn &%s; }\n" % (classname, use, objname))
		file.write("\n}\n")


class Typedef(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'public')

class Namespace(Container):
	def __init__(self, parent, name, line):
		Container.__init__(self, parent, name, line, 'published')

	def dump(self, file, namespace, nested):
		if showline: file.write("#line %d\n" % self.line)
		file.write("namespace %s\n" % self.name)
		file.write("{\n")
		classes = Container.dump(self, file, namespace + '::' + self.name, False)
		file.write("}\n")
		return classes

	def predecl(self, file):
		if self.name != 'BugEngine':
			file.write("using namespace %s;\n"%self.fullname)
		file.write("::BugEngine::RTTI::ClassInfo* be_Namespace%s();\n"%"_".join(self.fullname.split('::')))
		Container.predecl(self, file)



class Enum(Container):
	def __init__(self, parent, name, line, scope):
		Container.__init__(self, parent, name, line, scope)
		self.values = []
		self.tags = []

	def addEnumValue(self, name):
		self.values.append(name)

	def predecl(self, file):
		file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass();\n" % (self.fullname))

	def dump(self, file, namespace, nested):
		name = self.fullname[2:].replace('::', '.')
		decl = "enum%s" % self.fullname.replace(':', '_')
		tagname = self.writeTags(file, decl, self.tags)
		if self.useMethods:
			file.write("static const ::BugEngine::RTTI::ClassInfo& s_%sFun()\n{\n" % decl)
		hash = self.hash.hexdigest()
		file.write("static const ::BugEngine::RTTI::ClassInfo s_%s =\n" % (decl))
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
		file.write("        0,\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % self.fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >,\n" % self.fullname)
		file.write("        {{ 0x%s, 0x%s, 0x%s, 0x%s }}\n" % (hash[0:8], hash[8:16], hash[16:24], hash[24:32]))
		file.write("    };\n")
		if not nested:
			classname = self.parent.fullname.split('::')[1:]
			owner = "be_Namespace"
			for ns in classname:
				owner += "_%s" % ns
			file.write("static ::BugEngine::RTTI::ClassInfo::ObjectInfo s_%s_obj = { %s()->objects, \"%s\", ::BugEngine::Value(&s_%s) };\n" % (decl, owner, self.name, decl))
			file.write("static const ::BugEngine::RTTI::ClassInfo::ObjectInfo* s_%s_obj_ptr = ( %s()->objects = &s_%s_obj );\n" % (decl, owner, decl))
		if self.useMethods:
			file.write("return s_%s;\n}\n" % decl)
			return [(self.fullname, "%s::s_%sFun()" % (namespace, decl), nested)]
		else:
			return [(self.fullname, "%s::s_%s" % (namespace, decl), nested)]

class Class(Container):
	def __init__(self, parent, name, inherits, line, scope, value):
		Container.__init__(self, parent, name, line, scope)
		self.inherits = inherits or 'void'
		self.value = value
		self.tags = []

	def predecl(self, file):
		Container.predecl(self, file)
		file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass();\n" % (self.fullname))
		file.write("template< > const RTTI::ClassInfo* be_typeid< %s >::klass();\n" % (self.inherits))

	def dump(self, file, namespace, nested):
		file.write("//----------------------------------------------------------------------\n")
		file.write("// %s\n" % self.fullname)
		self.hash.update(self.fullname)
		classes = Container.dump(self, file, namespace, True)
		decl = "class%s" % self.fullname.replace(':', '_')
		if self.useMethods:
			file.write("static const ::BugEngine::RTTI::ClassInfo& s_%sFun()\n{\n" % decl)
		properties = self.buildProperties(file, decl)
		statics = self.buildStatics(file, decl)
		methods, constructor, call = self.buildMethods(file, decl);
		self.writeClass(file, decl, nested, properties, methods, statics, constructor, call)
		if self.useMethods:
			file.write("return s_%s;\n}\n" % decl)
			classes.append((self.fullname, "%s::s_%sFun()" % (namespace, decl), nested))
		else:
			classes.append((self.fullname, "%s::s_%s" % (namespace, decl), nested))
		file.write("//----------------------------------------------------------------------\n\n")
		return classes

	def writeClass(self, file, decl, nested, properties, methods, objects, constructor, call):
		name = self.fullname[2:].replace('::', '.')
		tagname = self.writeTags(file, decl, self.tags)
		hash = self.hash.hexdigest()
		file.write("static const ::BugEngine::RTTI::ClassInfo s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (name))
		file.write("        be_typeid< %s >::klass(),\n" % (self.inherits))
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % self.fullname)
		file.write("        be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1),\n" % (self.inherits, self.fullname))
		file.write("        %s,\n" % (tagname))
		file.write("        %s,\n" % (properties))
		file.write("        %s,\n" % (methods))
		file.write("        %s,\n" % (objects))
		file.write("        %s,\n" % (constructor))
		file.write("        %s,\n" % (call))
		if self.value:
			file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % self.fullname)
			file.write("        &::BugEngine::RTTI::wrapDestroy< %s >,\n" % self.fullname)
		else:
			file.write("        0,\n")
			file.write("        0,\n")
		file.write("        {{ 0x%s, 0x%s, 0x%s, 0x%s }}\n" % (hash[0:8], hash[8:16], hash[16:24], hash[24:32]))
		file.write("    };\n")
		if not nested:
			classname = self.parent.fullname.split('::')[1:]
			owner = "be_Namespace"
			for ns in classname:
				owner += "_%s" % ns
			file.write("static ::BugEngine::RTTI::ClassInfo::ObjectInfo s_%s_obj = { %s()->objects, \"%s\", ::BugEngine::Value(&s_%s) };\n" % (decl, owner, self.name, decl))
			file.write("static const ::BugEngine::RTTI::ClassInfo::ObjectInfo* s_%s_obj_ptr = ( %s()->objects = &s_%s_obj );\n" % (decl, owner, decl))

	def buildProperties(self, file, decl):
		prop = "0"
		for type,name,attr,visibility,tags,line in self.members[::-1]:
			if visibility == 'published':
				if showline:
					file.write("#line %d\n" % (line))
				self.hash.update("property")
				self.hash.update(type)
				self.hash.update(name)
				tagname = self.writeTags(file, decl+"_"+name, tags)
				file.write("static const ::BugEngine::RTTI::PropertyInfo s_%s_%s =\n" % (decl, name))
				file.write("    {\n")
				file.write("        %s,\n" % tagname)
				file.write("        %s,\n" % prop)
				file.write("        \"%s\",\n" % name)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % self.fullname)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % type)
				file.write("        be_checked_numcast<u32>((char*)(&((%s*)0)->%s)-(char*)0)\n" % (self.fullname, name))
				file.write("    };\n")
				prop = "&s_%s_%s" % (decl, name)
		return prop

	def buildStatics(self, file, decl):
		return "be_typeid< %s >::klass()->objects"%self.inherits
		file.write("static const ::BugEngine::RTTI::ClassInfo::ObjectInfo s_%s_%s =\n" % (decl, prettyname))
		file.write("    {\n")
		file.write("        %s,\n" % method)
		file.write("        \"%s\",\n" % name)
		file.write("        ::BugEngine::Value(&s_method_%s_%s)\n" % (decl, prettyname))
		file.write("    };\n")


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
			self.hash.update("method")
			self.hash.update(name)

			for rtype, params, attrs, visibility, tags, line in overloads:
				self.hash.update("overload")
				if "const" in attrs:
					self.hash.update("const")
				if "static" in attrs:
					self.hash.update("static")
				paramindex = 0
				param = "0"
				if showline: file.write("#line %d\n" % (line))
				method_tagname = self.writeTags(file, decl+"_"+prettyname, tags)
				for ptype, pname, tags in params[::-1]:
					self.hash.update("param")
					self.hash.update("ptype")
					self.hash.update("pname")
					param_tagname = self.writeTags(file, decl+"_"+prettyname+"_"+pname, tags)
					file.write("static const ::BugEngine::RTTI::MethodInfo::OverloadInfo::ParamInfo s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        %s,\n" % param_tagname)
					file.write("        %s,\n" % param)
					file.write("        \"%s\",\n" % pname)
					file.write("        ::BugEngine::be_typeid< %s >::type()\n" % ptype)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)
					paramindex = paramindex + 1
				if 'static' not in attrs:
					file.write("static const ::BugEngine::RTTI::MethodInfo::OverloadInfo::ParamInfo s_%s_%s_%d_p%d =\n" % (decl, prettyname, overloadindex, paramindex))
					file.write("    {\n")
					file.write("        0,\n")
					file.write("        %s,\n" % param)
					file.write("        \"this\",\n")
					if "const" in attrs:
						file.write("        ::BugEngine::be_typeid< %s const* >::type()\n" % self.fullname)
					else:
						file.write("        ::BugEngine::be_typeid< %s * >::type()\n" % self.fullname)
					file.write("    };\n")
					param = "&s_%s_%s_%d_p%d" % (decl, prettyname, overloadindex, paramindex)

				paramtypes = ', '.join(ptype for ptype, pname, tags in params)
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
				file.write("        %s,\n" % method_tagname)
				file.write("        %s,\n" % overload)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % rtype)
				file.write("        %s,\n" % param)
				file.write("        %s::VarArg,\n" % helper)
				file.write("        %s\n" % callptr)
				file.write("    };\n")
				overload = "&s_%s_%s_%d" % (decl, prettyname, overloadindex)
				overloadindex = overloadindex + 1

			file.write("static const ::BugEngine::RTTI::MethodInfo s_method_%s_%s =\n" % (decl, prettyname))
			file.write("    {\n")
			file.write("        \"%s\",\n" % name)
			file.write("        %s,\n" % method)
			file.write("        %s\n" % overload)
			file.write("    };\n")
			if name == "?ctor":
				constructor = "&s_method_%s_%s" % (decl, prettyname)
			elif name == "?call":
				call = "&s_method_%s_%s" % (decl, prettyname)
			method = "&s_method_%s_%s" % (decl, prettyname)

		return method, constructor, call
