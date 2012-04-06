import cpp.yacc as yacc
import rtti

class Members(yacc.Nonterm):
	"%nonterm"
	def members(self, skip_list):
		"%reduce SkipList"




class Visibility(yacc.Nonterm):
	"%nonterm"

	def private(self, private):
		"%reduce PRIVATE"
		self.visibility = 1
	def protected(self, protected):
		"%reduce PROTECTED"
		self.visibility = 2
	def public(self, public):
		"%reduce PUBLIC"
		self.visibility = 3
	def none(self):
		"%reduce"
		self.visibility = 4
	def published(self, published):
		"%reduce PUBLISHED"
		self.visibility = 5




class ParentList(yacc.Nonterm):
	"%nonterm"

	def parent_item(self, visibility, name):
		"%reduce Visibility Name"
		self.inherits = (visibility.visibility, name.value)
	def parent_list(self, parent_list, comma, visibility, name):
		"%reduce ParentList COMMA Visibility Name"
		if visibility.visibility > parent_list.inherits[0]:
			self.inherits = (visibility.visibility, name.value)
		else:
			self.inherits = parent_list.inherits




class Parent(yacc.Nonterm):
	"%nonterm"

	def parent_none(self):
		"%reduce"
		self.inherits = (0, "")
	def parent_list(self, colon, parent_list):
		"%reduce COLON ParentList"
		self.inherits = parent_list.inherits




class ClassDef(yacc.Nonterm):
	"%nonterm"

	def class_definition(self, cls, name, parent, lbrace, members, rbrace, semi):
		"%reduce CLASS Name Parent LBRACE Members RBRACE SEMI"
		if parent.inherits[0] >= 5:
			inherits = parent.inherits[1]
		else:
			inherits = ""
		print(inherits)
		self.value = rtti.Class(name.value, inherits, cls.lineno, False)

	def struct_definition(self, cls, name, parent, lbrace, members, rbrace, semi):
		"%reduce STRUCT Name Parent LBRACE Members RBRACE SEMI"
		if parent.inherits[0] >= 4:
			inherits = parent.inherits[1]
		else:
			inherits = ""
		print(inherits)
		self.value = rtti.Class(name.value, inherits, cls.lineno, True)

	def union_definition(self, union, name, lbrace, members, rbrace, semi):
		"%reduce UNION Name LBRACE Members RBRACE SEMI"
		self.value = rtti.Class(name.value, "", union.lineno, True)
