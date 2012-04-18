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

	def predecl(self, file, instances, name, member):
		name = name+[self.name]
		instances.write("namespace %s\n" % self.name)
		instances.write("{\n")
		owner = "be_%s_Namespace"%self.parser.plugin
		for ns in name:
			owner += "_%s" % ns
		owner += "()"
		file.write("raw< ::BugEngine::RTTI::Class > be_%s_Namespace_%s();\n"%(self.parser.plugin, "_".join(name)))
		self.exprs.predecl(file, instances, name, member)
		instances.write("\n}\n")

	def dump(self, file, instances, namespace, name, member, object_ptr):
		namespace = name+[self.name]
		name = name+[self.name]
		file.write("namespace %s\n" % self.name)
		file.write("{\n")

		owner = ("be_%s_Namespace_"%self.parser.plugin) + "_".join(name) + "()"
		my_object_ptr, method_ptr, constructor, call = self.exprs.dump(file, instances, namespace, name, "", True)
		if my_object_ptr != '%s->objects'%owner:
			file.write("const ::BugEngine::RTTI::Class::ObjectInfo* %s_ptr = ( %s->objects.set(%s) );\n" % (my_object_ptr[2:-1], owner, my_object_ptr[1:-1]))
		file.write("\n}\n")
		return object_ptr


