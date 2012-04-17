import cpp


class EnumDef(cpp.yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, skip, rbrace):
		"%reduce ENUM NameOpt LBRACE SkipList RBRACE"
		self.name = name.value
		self.value = None
		self.lineno = name.lineno

	def predecl(self, file, instances, name, member):
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		decl = "enum%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))

	def dump(self, file, instances, namespace, name, member, object_ptr):
		ns = '::'+'::'.join(namespace)
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		prettyname = '.'.join(name)

		tagname = "{0}"
		properties = "{0}"
		objects = "{0}"
		methods = constructor = destructor = call = "0"
		decl = "enum%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, decl)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		else:
			varname = "%s::s_%s" % (ns, decl)

		file.write("::BugEngine::RTTI::Class s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (prettyname))
		file.write("        be_typeid< void >::klass(),\n")
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % fullname)
		file.write("        0,\n")
		file.write("        %s,\n" % (tagname))
		file.write("        %s,\n" % (properties))
		file.write("        {%s},\n" % (methods))
		file.write("        %s,\n" % (objects))
		file.write("        {0},\n")
		file.write("        {0},\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % fullname)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % decl)
		file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj = { %s, %s, \"%s\", ::BugEngine::RTTI::Value(&%s) };\n" % (decl, object_ptr, tagname, self.name, varname))

		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return "{&s_%s_obj}"%decl
