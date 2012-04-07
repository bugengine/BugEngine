import cpp.yacc as yacc
import rtti


class EnumDef(yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, id, lbrace, skip, rbrace):
		"%reduce ENUM Name LBRACE SkipList RBRACE"
		self.value = None
