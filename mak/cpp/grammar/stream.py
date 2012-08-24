import cpp


class StreamDef(cpp.yacc.Nonterm):
	"%nonterm"

	def stream_definition(self, stream, lparen, name, rparen):
		"%reduce BE_PRODUCT LPAREN ID RPAREN"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = stream.lineno

	def using(self, file, instances, decl, name, parent_name):
		pass

	def predecl(self, file, instances, decl, name, member):
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = '::'+'::'.join(name)
		prefix = "stream%s" % '_'.join(decl)
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % prefix)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (prefix))


	def dump(self, file, instances, namespace, owner, decl, name, member):
		ns = '::'.join(namespace)
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = '::'.join(name)
		prettyname = '.'.join(name)
		prefix = "stream%s" % '_'.join(decl)

		if member:
			file.write("#line %d\n"%self.lineno)
			file.write("typedef %s %s;\n" % ('::'.join(name), self.name))

		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, prefix)
			file.write("#line %d\n"%self.lineno)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun ()\n{\n" % prefix)
		else:
			varname = "%s::s_%s" % (ns, prefix)

		tag_ptr = self.tags.dump(file, instances, prefix)

		file.write("#line %d\n"%self.lineno)
		if self.parser.useMethods:
			file.write("static ")
		file.write("::BugEngine::RTTI::Class s_%s =\n" % (prefix))
		file.write("#line %d\n"%self.lineno)
		file.write("    {\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::istring(\"Product<%s>\"),\n" % (self.name))
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::be_game_Namespace_BugEngine_Kernel(),\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::be_typeid< void >::klass(),\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        (u32)(sizeof(BugEngine::Kernel::Product< %s >)),\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        (i32)(0),\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % tag_ptr)
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        0,\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        0\n")
		file.write("#line %d\n"%self.lineno)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % prefix)

		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< BugEngine::Kernel::Product<%s> >::klass() { raw<const RTTI::Class> ci = {&s_%s}; return ci; }\n" % (fullname, prefix))

		return varname, "{0}"

