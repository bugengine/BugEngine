import cpp.yacc as yacc
import rtti


class EnumDef(yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, skip, rbrace):
		"%reduce ENUM Name LBRACE SkipList RBRACE"
		self.name = name.value
		self.value = None
		self.lineno = name.lineno

	def predecl(self, file, instances, name, parent, member):
		name = name+[self.name]

	def dump(self, file, instances, name, parent, member):
		name = name+[self.name]
