import cpp


class EnumDef(cpp.yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, skip, rbrace):
		"%reduce ENUM NameOpt LBRACE SkipList RBRACE"
		self.name = name.value
		self.value = None
		self.lineno = enum.lineno

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
		decl = "enum%s" % fullname.replace(':', '_')

		tag_ptr = self.tags.dump(file, instances, decl)
		properties = "{0}"
		objects = "{0}"
		methods = constructor = destructor = call = "0"
		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, decl)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		else:
			varname = "%s::s_%s" % (ns, decl)

		file.write("#line %d\n"%self.lineno)
		file.write("::BugEngine::RTTI::Class s_%s =\\\n" % (decl))
		file.write("    {\\\n")
		file.write("        inamespace(\"%s\"),\\\n" % (prettyname))
		file.write("        be_typeid< void >::klass(),\\\n")
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\\\n" % fullname)
		file.write("        0,\\\n")
		file.write("        %s,\\\n" % (tag_ptr))
		file.write("        %s,\\\n" % (properties))
		file.write("        {%s},\\\n" % (methods))
		file.write("        %s,\\\n" % (objects))
		file.write("        {0},\\\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\\\n" % fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\\\n" % fullname)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % decl)

		alias_index = 0
		for name in self.aliases+[self.name]:
			alias_index += 1
			file.write("#line %d\n"%self.lineno)
			file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj_%d = { %s, %s, \"%s\", ::BugEngine::RTTI::Value(&%s) };\n" % (decl, alias_index, object_ptr, tag_ptr, name, varname))
			object_ptr = "{&s_%s_obj_%d}"%(decl,alias_index)


		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return object_ptr
