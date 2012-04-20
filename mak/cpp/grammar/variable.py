import cpp


class VariableItem(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, type, name):
		"%reduce Type ID"
		self.type = type.value
		self.name = name.value

	def method_ptr(self, method_ptr):
		"%reduce MethodPointer"
		self.type = method_ptr.type
		self.name = method_ptr.name

class VariableAttributes(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableItem"
		self.type = variable.type
		self.name = variable.name
		self.attributes = set([])

	def static_variable(self, static, variable):
		"%reduce STATIC VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.attributes.add('static')

	def mutable_variable(self, static, variable):
		"%reduce MUTABLE VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes
		self.attributes.add('mutable')

class Variable(cpp.yacc.Nonterm):
	"%nonterm"

	def variable(self, variable):
		"%reduce VariableAttributes"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes

	def variable_array(self, variable, lbracket, value, rbracket):
		"%reduce VariableAttributes LBRACKET Value RBRACKET"
		self.type = variable.type + '['+value.value+']'
		self.name = variable.name
		self.attributes = variable.attributes

	def variable_value(self, variable, eq, value):
		"%reduce VariableAttributes EQUAL Value"
		self.type = variable.type
		self.name = variable.name
		self.attributes = variable.attributes

	def dump(self, file, instances, namespace, name, parent_name, property_ptr, object_ptr):
		if name:
			fullname = '::'+'::'.join(name)
		else:
			fullname = ""
		decl = '_'.join(name)
		tag_ptr = self.tags.dump(file, instances, decl)
		index = 0
		for name in [self.name]+self.aliases:
			if not parent_name or 'static' in self.attributes:
				file.write("static const ::BugEngine::RTTI::Class::ObjectInfo s_%s_%s_%d =\n" % (decl, self.name, index))
				file.write("    {\n")
				file.write("        %s,\n" % object_ptr)
				file.write("        %s,\n" % tag_ptr)
				file.write("        \"%s\",\n" % name)
				file.write("        ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s::%s))\n" % (fullname, self.name))
				file.write("    };\n")
				object_ptr = "{&s_%s_%s_%d}" % (decl, self.name, index)
				index = index + 1
			else:
				file.write("static const ::BugEngine::RTTI::Property s_%s_%s_%d =\n" % (decl, self.name, index))
				file.write("    {\n")
				file.write("        %s,\n" % tag_ptr)
				file.write("        %s,\n" % property_ptr)
				file.write("        \"%s\",\n" % name)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % fullname)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % self.type)
				file.write("        &::BugEngine::RTTI::PropertyHelper<%s, %s, &%s::%s>::get\n" % (self.type, fullname, fullname, self.name))
				file.write("    };\n")
				property_ptr = "{&s_%s_%s_%d}" % (decl, self.name, index)
				index = index + 1
		return property_ptr, object_ptr
