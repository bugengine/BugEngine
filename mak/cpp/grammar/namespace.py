import cpp

class Namespace(cpp.yacc.Nonterm):
	"%nonterm"
	def namespace_named(self, namespace, name, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE ID LBRACE Exprs RBRACE"
		self.name = name.value
		self.decl = name.value
		self.members = exprs
		self.aliases = []
		self.lineno = name.lineno

	def namespace_unnamed(self, namespace, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE LBRACE RBRACE"
		#unnamed namespaces are not exported
		self.name = ''
		self.decl = '__unnamed__'
		self.aliases = []
		self.lineno = namespace.lineno
		self.members = None

	def predecl(self, files, namespace, parent):
		namespace = namespace + [self.name]
		if self.members:
			files[0].write('namespace %s\n{\n\n' % self.name)
			files[1].write('namespace %s\n{\n\n' % self.name)
			self.members.predecl(files, namespace, [], False)
			files[0].write('\n}\n')
			files[1].write('\n}\n')

	def dump(self, files, namespace, parent):
		namespace = namespace + [self.name]
		owner = '::BugEngine::be_%s_Namespace_%s()' % (self.parser.plugin, '_'.join(namespace))
		if self.members:
			files[0].write('namespace %s\n{\n' % self.name)
			self.members.dumpObjects(files, namespace, parent)
			objects, methods, properties = self.members.dump(files, namespace, parent, owner)

			if objects:
				files[0].write('static raw<const ::BugEngine::RTTI::ObjectInfo> const s_%s_namespace_%s_obj = %s;\n' % (self.name, self.members.objects[0].name, objects))
				files[0].write('static const ::BugEngine::RTTI::ObjectInfo* const s_%s_namespace_%s =\n' % (self.name, self.members.objects[0].name))
				files[0].write('	%s->objects.set(s_%s_namespace_%s_obj.operator->());\n' % (owner, self.name, self.members.objects[0].name))

			files[0].write('}\n')
			

