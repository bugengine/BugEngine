import cpp

class Namespace(cpp.yacc.Nonterm):
	"%nonterm"
	def namespace_named(self, namespace, name, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE ID LBRACE Exprs RBRACE"
		self.name = name.value
		self.exprs = exprs

	def namespace_unnamed(self, namespace, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE LBRACE RBRACE"
		#unnamed namespaces are not exported
		self.name = ''

	def predecl(self, file, instances, name, owner, member):
		name = name+[self.name]
		instances.write("namespace %s\n" % self.name)
		instances.write("{\n")
		owner = "be_%s_Namespace"%self.parser.plugin
		for ns in name:
			owner += "_%s" % ns
		owner += "()"
		file.write("raw< ::BugEngine::RTTI::Class > be_%s_Namespace_%s();\n"%(self.parser.plugin, "_".join(name)))
		self.exprs.predecl(file, instances, name, owner, member)
		instances.write("\n}\n")

	def dump(self, file, instances, name, owner, member):
		name = name+[self.name]
		owner = "be_%s_Namespace"%self.parser.plugin
		file.write("namespace %s\n" % self.name)
		file.write("{\n")

		for ns in name:
			owner += "_%s" % ns
		owner += "()"
		self.exprs.dump(file, instances, name, owner, "")

		file.write("\n}\n")


