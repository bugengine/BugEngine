import cpp



class BuiltinDef(cpp.yacc.Nonterm):
	"%nonterm"

	def builtin(self, builtin, name, lbrace, rbrace):
		"%reduce BUILTIN Name LBRACE RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = builtin.lineno

	def using(self, file, instances, decl, name, parent_name):
		pass

	def predecl(self, file, instances, decl, name, member):
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = "builtin%s" % '_'.join(decl)
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % fullname)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (fullname))

	def dump(self, file, instances, namespace, decl, name, member):
		ns = '::'.join(namespace)
		owner = '::'.join(name)
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = '::'.join(name)
		prettyname = '.'.join(name)
		prefix = "builtin%s" % '_'.join(decl)

		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, prefix)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % prefix)
		else:
			varname = "%s::s_%s" % (ns, prefix)

		tag_ptr = self.tags.dump(file, instances, prefix)
		properties = "{0}"
		objects = "{0}" #self.value.dump(file, instances, prefix, owner, self.name)
		methods = "{0}"

		file.write("#line %d\n"%self.lineno)
		if self.parser.useMethods:
			file.write("static ")
		file.write("::BugEngine::RTTI::Class s_%s =\n" % (prefix))
		file.write("#line %d\n"%self.lineno)
		file.write("    {\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::inamespace(\"%s\"),\n" % (prettyname))
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::be_typeid< void >::klass(),\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        be_checked_numcast<u16>(sizeof(%s)),\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        be_checked_numcast<u16>(be_alignof(%s)),\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        0,\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (tag_ptr))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (properties))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (methods))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (objects))
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("#line %d\n"%self.lineno)
			file.write("return s_%s;\n}\n" % prefix)

		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return varname, tag_ptr
