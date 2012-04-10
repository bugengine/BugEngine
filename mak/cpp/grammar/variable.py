import cpp


class VariableItem(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, type, name):
		"%reduce Type ID"

class VariableAttributes(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableItem"
	def static_variable(self, static, variable):
		"%reduce STATIC VariableAttributes"

class Variable(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableAttributes"
	def variable_value(self, variable, eq, value):
		"%reduce VariableAttributes EQUAL Value"

