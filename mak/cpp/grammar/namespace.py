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

	def using(self, files, namespace, owner):
		if self.members:
			self.members.using(files, namespace, owner)

	def predecl(self, files, namespace, owner):
		if self.members:
			self.members.predecl(files, namespace, owner)

	def dump(self, files, namespace, owner):
		if self.members:
			self.members.dumpObjects(files, namespace, owner)
			self.members.dump(files, namespace, owner)

