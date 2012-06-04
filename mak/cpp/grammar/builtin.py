import cpp



class BuiltinDef(cpp.yacc.Nonterm):
	"%nonterm"

	def builtin(self, builtin, name, lbrace, rbrace):
		"%reduce BUILTIN Name LBRACE RBRACE"
		self.name = name.value
		self.lineno = builtin.lineno

	def predecl(self, file, instances, name, member):
		name = name+[self.name]
		fullname = '::'.join(name)
		decl = "builtin%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))

	def dump(self, file, instances, namespace, name, member):
		ns = '::'.join(namespace)
		owner = '::'.join(name)
		name = name+[self.name]
		fullname = '::'.join(name)
		prettyname = '.'.join(name)
		decl = "builtin%s" % fullname.replace(':', '_')

		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, decl)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		else:
			varname = "%s::s_%s" % (ns, decl)

		tag_ptr = self.tags.dump(file, instances, decl)
		properties = "{0}"
		objects = "{0}" #self.value.dump(file, instances, decl, owner, self.name)
		methods = "{0}"

		file.write("#line %d\n"%self.lineno)
		if self.parser.useMethods:
			file.write("static ")
		file.write("::BugEngine::RTTI::Class s_%s =\\\n" % (decl))
		file.write("    {\\\n")
		file.write("        ::BugEngine::inamespace(\"%s\"),\\\n" % (prettyname))
		file.write("        ::BugEngine::be_typeid< void >::klass(),\\\n")
		file.write("        be_checked_numcast<u16>(sizeof(%s)),\\\n" % fullname)
		file.write("        be_checked_numcast<u16>(be_alignof(%s)),\\\n" % fullname)
		file.write("        0,\\\n")
		file.write("        %s,\\\n" % (tag_ptr))
		file.write("        %s,\\\n" % (properties))
		file.write("        %s,\\\n" % (methods))
		file.write("        %s,\\\n" % (objects))
		file.write("        {0},\\\n")
		file.write("        {0},\\\n")
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\\\n" % fullname)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\\\n" % fullname)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % decl)

		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return varname, tag_ptr
