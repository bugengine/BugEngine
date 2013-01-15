import cpp

class Enum(cpp.yacc.Nonterm):
	"%nonterm"

	def enum_value(self, tags_left, id, equal, value, tags_right):
		"%reduce TagsLeft ID EQUAL Value TagsRight"
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags

	def enum(self, tags_left, id, tags_right):
		"%reduce TagsLeft ID TagsRight"
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags



class EnumSequence(cpp.yacc.Nonterm):
	"%nonterm"

	def enumseq(self, enumseq, comma, tags, enum):
		"%reduce EnumSequence COMMA TagsRight Enum"
		self.enums = enumseq.enums
		self.enums.append(enum)
		enum.tags.tags += tags.tags

	def enumseq_1(self, enum):
		"%reduce Enum"
		self.enums = [enum]



class EnumValueList(cpp.yacc.Nonterm):
	"%nonterm"

	def empty(self):
		"%reduce"
		self.enums = []

	def enum_list(self, enums_sequence):
		"%reduce EnumSequence"
		self.enums = enums_sequence.enums

class EnumDef(cpp.yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, enums, rbrace):
		"%reduce ENUM NameOpt LBRACE EnumValueList RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = self.name
		self.value = enums
		self.lineno = enum.lineno

	def using(self, files, namespace, owner):
		pass

	def predecl(self, files, namespace, owner):
		pass

	def dump(self, files, namespace, owner):
		pass
