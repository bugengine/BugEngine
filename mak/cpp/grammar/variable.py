import cpp


class VariableItem(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, type, name):
		"%reduce Type ID"
		self.type = type.value
		self.name = name.value
		self.lineno = name.lineno

	def method_ptr(self, method_ptr):
		"%reduce MethodPointer"
		self.type = method_ptr.type
		self.name = method_ptr.name
		self.lineno = method_ptr.lineno

class VariableAttributes(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableItem"
		self.type = variable.type
		self.name = variable.name
		self.attributes = set([])
		self.lineno = variable.lineno

	def static_variable(self, static, variable):
		"%reduce STATIC VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.attributes.add('static')
		self.lineno = variable.lineno

	def mutable_variable(self, static, variable):
		"%reduce MUTABLE VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.attributes.add('mutable')
		self.lineno = variable.lineno

class Variable(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.lineno = variable.lineno

	def variable_array(self, variable, lbracket, value, rbracket):
		"%reduce VariableAttributes LBRACKET Value RBRACKET"
		self.type = variable.type + '['+value.value+']'
		self.name = variable.name
		self.attributes = variable.attributes
		self.lineno = variable.lineno

	def variable_value(self, variable, eq, value):
		"%reduce VariableAttributes EQUAL Value"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.lineno = variable.lineno

	def dump(self, files, namespace, parent):
		if parent:
			owner = '::BugEngine::be_typeid< ::%s::%s >::klass()' % ('::'.join(namespace + parent))
		else:
			owner = '::BugEngine::be_%s_Namespace_%s()' % (self.parser.plugin, '_'.join(namespace))
		parent = parent + [self.name]
