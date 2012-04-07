import cpp.yacc as yacc


class TypeDef(yacc.Nonterm):
	"%nonterm"

	def type_class(self, cls):
		"%reduce ClassDef"
		self.value = cls.value

	def type_enum(self, cls):
		"%reduce EnumDef"
		self.value = cls.value



class TypeDecl(yacc.Nonterm):
	"%nonterm"

	def class_declaration(self, type, name):
		"%reduce CLASS Name"
		self.value = None

	def struct_declaration(self, type, name):
		"%reduce STRUCT Name"
		self.value = None

	def union_declaration(self, type, name):
		"%reduce UNION Name"
		self.value = None

	def enum_declaration(self, type, name):
		"%reduce ENUM Name"
		self.value = None




class TypePart(yacc.Nonterm):
	"%nonterm"

	def type_declaration(self, type):
		"%reduce TypeDecl"
		self.value = None

	def type_name(self, type):
		"%reduce Name"
		self.value = None

	def type_ptr(self, type, ptr):
		"%reduce TypePart TIMES"
		self.value = None

	def type_ref(self, type, ref):
		"%reduce TypePart AND"
		self.value = None

	def type_const(self, type, const):
		"%reduce TypePart CONST"
		self.value = None




class Type(yacc.Nonterm):
	"%nonterm"

	def type(self, part):
		"%reduce TypePart"
		self.value = None
	def type_const(self, const, part):
		"%reduce CONST TypePart"
		self.value = None
