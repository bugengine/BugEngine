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

	def dump(self, files, namespace, parent, owner, previous_object, previous_property):
		if parent and not 'static' in self.attributes:
			tags = self.tags.dump(files, 's_%s_property' % self.name)
			for name in [self.name] + self.tags.aliases:
				pretty_name = name.replace('?', '_').replace('#', '_')
				new_property = 's_%s_property' % pretty_name
				files[0].write('static const ::BugEngine::RTTI::Property %s =\n' % new_property)
				files[0].write('{\n')
				files[0].write('	%s,\n' % tags)
				files[0].write('	%s,\n' % previous_property)
				files[0].write('	::BugEngine::istring("%s"),\n' % name)
				files[0].write('	::BugEngine::be_typeid< %s >::type(),\n' % '::'.join(parent))
				files[0].write('	::BugEngine::be_typeid< %s >::type(),\n' % self.type)
				files[0].write('	&::BugEngine::RTTI::PropertyHelper< %s, %s, &%s::%s >::get\n' % (self.type, '::'.join(parent), '::'.join(parent), self.name))
				files[0].write('};\n')
				previous_property = '{&%s}' % new_property
		elif parent or not 'static' in self.attributes:
			tags = self.tags.dump(files, 's_%s_property' % self.name)
			for name in [self.name] + self.tags.aliases:
				pretty_name = name.replace('?', '_').replace('#', '_')
				new_object = 's_%s_property' % pretty_name
				files[0].write('static const ::BugEngine::RTTI::ObjectInfo %s =\n' % new_object)
				files[0].write('{\n')
				files[0].write('	%s,\n' % previous_object)
				files[0].write('	%s,\n' % tags)
				files[0].write('	::BugEngine::istring("%s"),\n' % name)
				files[0].write('	::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s::%s))\n' % ('::'.join(parent), self.name))
				files[0].write('};\n')
				previous_object = '{&%s}' % new_object

		return previous_object, previous_property
