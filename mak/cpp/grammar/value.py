import cpp.yacc as yacc

class Value(yacc.Nonterm):
	"%nonterm"

	def constant_string(self, constant):
		"%reduce STRING"
		self.value = constant.value
	def constant_wstring(self, constant):
		"%reduce WSTRING"
		self.value = constant.value
	def constant_charconst(self, constant):
		"%reduce CHARCONST"
		self.value = constant.value
	def constant_wchar(self, constant):
		"%reduce WCHAR"
		self.value = constant.value
	def constant_decimal(self, constant):
		"%reduce DECIMAL"
		self.value = constant.value
	def constant_octal(self, constant):
		"%reduce OCTAL"
		self.value = constant.value
	def constant_hex(self, constant):
		"%reduce HEX"
		self.value = constant.value
	def constant_floating(self, constant):
		"%reduce FLOATING"
		self.value = constant.value


class VariableItem(yacc.Nonterm):
	"%nonterm"

	def variable(self, type, name):
		"%reduce Type ID"

class VariableAttributes(yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableItem"
	def static_variable(self, static, variable):
		"%reduce STATIC VariableAttributes"

class Variable(yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableAttributes"
	def variable_value(self, variable, eq, value):
		"%reduce VariableAttributes EQUAL Value"