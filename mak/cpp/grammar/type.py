import cpp


class TypeDef(cpp.yacc.Nonterm):
	"%nonterm"

	def type_class(self, cls):
		"%reduce ClassDef"
		self.value = cls

	def type_enum(self, cls):
		"%reduce EnumDef"
		self.value = cls



class TypeDecl(cpp.yacc.Nonterm):
	"%nonterm"

	def class_declaration(self, type, name):
		"%reduce CLASS Name"
		self.lineno = name.lineno
		self.value = None

	def struct_declaration(self, type, name):
		"%reduce STRUCT Name"
		self.lineno = name.lineno
		self.value = None

	def union_declaration(self, type, name):
		"%reduce UNION Name"
		self.lineno = name.lineno
		self.value = None

	def enum_declaration(self, type, name):
		"%reduce ENUM Name"
		self.lineno = name.lineno
		self.value = None




class TypePart(cpp.yacc.Nonterm):
	"%nonterm"

	def type_declaration(self, type):
		"%reduce TypeDecl"
		self.lineno = type.lineno
		self.value = None

	def type_name(self, type):
		"%reduce Name"
		self.lineno = type.lineno
		self.value = None

	def type_ptr(self, type, ptr):
		"%reduce TypePart TIMES"
		self.lineno = type.lineno
		self.value = None

	def type_ref(self, type, ref):
		"%reduce TypePart AND"
		self.lineno = type.lineno
		self.value = None

	def type_const(self, type, const):
		"%reduce TypePart CONST"
		self.lineno = type.lineno
		self.value = None




class Type(cpp.yacc.Nonterm):
	"%nonterm"

	def type(self, part):
		"%reduce TypePart"
		self.lineno = part.lineno
		self.value = None
	def type_const(self, const, part):
		"%reduce CONST TypePart"
		self.lineno = part.lineno
		self.value = None
