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

	def dump(self, file, instances, namespace, decl, name, parent_name, property_ptr, object_ptr):
		if name:
			fullname = '::'+'::'.join(name)
		else:
			fullname = ""
		decl = decl+[self.name]
		prefix = '_'.join(decl)
		tag_ptr = self.tags.dump(file, instances, prefix)
		index = 0
		for name in [self.name]+self.aliases:
			if not parent_name or 'static' in self.attributes:
				file.write("#line %d\n" % self.lineno)
				file.write("static const ::BugEngine::RTTI::ObjectInfo s_%s_%s_%d =\n" % (prefix, self.name, index))
				file.write("#line %d\n" % self.lineno)
				file.write("    {\n")
				file.write("#line %d\n" % self.lineno)
				file.write("        %s,\n" % object_ptr)
				file.write("#line %d\n" % self.lineno)
				file.write("        %s,\n" % tag_ptr)
				file.write("#line %d\n" % self.lineno)
				file.write("        \"%s\",\n" % name)
				file.write("#line %d\n" % self.lineno)
				file.write("        ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s::%s))\n" % (fullname, self.name))
				file.write("#line %d\n" % self.lineno)
				file.write("    };\n\n")
				object_ptr = "{&s_%s_%s_%d}" % (prefix, self.name, index)
				index = index + 1
			else:
				file.write("#line %d\n" % self.lineno)
				file.write("static const ::BugEngine::RTTI::Property s_%s_%s_%d =\n" % (prefix, self.name, index))
				file.write("#line %d\n" % self.lineno)
				file.write("    {\n")
				file.write("#line %d\n" % self.lineno)
				file.write("        %s,\n" % tag_ptr)
				file.write("#line %d\n" % self.lineno)
				file.write("        %s,\n" % property_ptr)
				file.write("#line %d\n" % self.lineno)
				file.write("        \"%s\",\n" % name)
				file.write("#line %d\n" % self.lineno)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % fullname)
				file.write("#line %d\n" % self.lineno)
				file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % self.type)
				file.write("#line %d\n" % self.lineno)
				file.write("        &::BugEngine::RTTI::PropertyHelper<%s, %s, &%s::%s>::get\n" % (self.type, fullname, fullname, self.name))
				file.write("#line %d\n" % self.lineno)
				file.write("    };\n\n")
				property_ptr = "{&s_%s_%s_%d}" % (prefix, self.name, index)
				index = index + 1
		return property_ptr, object_ptr
