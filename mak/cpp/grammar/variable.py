import cpp


class VariableItem(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, type, name):
		"%reduce Type ID"
	def method_ptr(self, method_ptr):
		"%reduce MethodPointer"

class VariableAttributes(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableItem"
	def static_variable(self, static, variable):
		"%reduce STATIC VariableAttributes"
	def mutable_variable(self, static, variable):
		"%reduce MUTABLE VariableAttributes"

class Variable(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableAttributes"
	def variable_array(self, variable, lbracket, value, rbracket):
		"%reduce VariableAttributes LBRACKET Value RBRACKET"
	def variable_value(self, variable, eq, value):
		"%reduce VariableAttributes EQUAL Value"

