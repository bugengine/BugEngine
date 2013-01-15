import cpp


class MemberList(cpp.yacc.Nonterm):
	"%nonterm"

	def empty(self):
		"%reduce"
		self.members = [None, None, None, None, None]

	def block(self, visibility, colon, exprs, memberlist):
		"%reduce Visibility COLON Exprs MemberList"
		self.members = memberlist.members
		if self.members[visibility.visibility]:
			self.members[visibility.visibility].members += exprs.members
			self.members[visibility.visibility].objects += exprs.objects
			for m, methods in exprs.methods.items():
				try:
					self.members[visibility.visibility].methods[m] += methods
				except KeyError:
					self.members[visibility.visibility].methods[m] = methods
		else:
			self.members[visibility.visibility] = exprs


class Members(cpp.yacc.Nonterm):
	"%nonterm"

	def list(self, exprs, members):
		"%reduce Exprs MemberList"
		self.members = members.members
		self.members[3] = exprs

class Visibility(cpp.yacc.Nonterm):
	"%nonterm"

	def private(self, private):
		"%reduce PRIVATE"
		self.visibility = 0
	def protected(self, protected):
		"%reduce PROTECTED"
		self.visibility = 1
	def none(self):
		"%reduce"
		self.visibility = 2
	def public(self, public):
		"%reduce PUBLIC"
		self.visibility = 3
	def published(self, published):
		"%reduce PUBLISHED"
		self.visibility = 4




class ParentList(cpp.yacc.Nonterm):
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




class Parent(cpp.yacc.Nonterm):
	"%nonterm"

	def parent_none(self):
		"%reduce"
		self.inherits = (0, "")
	def parent_list(self, colon, parent_list):
		"%reduce COLON ParentList"
		self.inherits = parent_list.inherits




class ClassDef(cpp.yacc.Nonterm):
	"%nonterm"

	def class_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce CLASS NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = cls.lineno
		self.value = False
		self.pod = False
		self.members = members.members[4]
		if parent.inherits[0] >= 3:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def struct_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce STRUCT NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = cls.lineno
		self.value = True
		self.pod = False
		self.members = members.members[3]
		if self.members and members.members[4]:
			self.members.members += members.members[4].members
			self.members.objects += members.members[4].objects
			for m, methods in members.members[4].methods.items():
				try:
					self.members.methods[m] += methods
				except KeyError:
					self.members.methods[m] = methods
		elif not self.members:
			self.members = members.members[4]
		if parent.inherits[0] >= 2:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"


	def pod_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce BE_POD NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = cls.lineno
		self.value = True
		self.pod = True
		self.members = members.members[3]
		if self.members and members.members[4]:
			self.members.members += members.members[4].members
			self.members.objects += members.members[4].objects
			for m, methods in members.members[4].methods.items():
				try:
					self.members.methods[m] += methods
				except KeyError:
					self.members.methods[m] = methods
		elif not self.members:
			self.members = members.members[4]
		if parent.inherits[0] >= 2:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def union_definition(self, union, name, lbrace, members, rbrace):
		"%reduce UNION NameOpt LBRACE Members RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
		self.lineno = union.lineno
		self.inherits = 'void'
		self.value = True
		self.pod = True
		self.members = members.members[3]
		if self.members and members.members[4]:
			self.members.members += members.members[4].members
			self.members.objects += members.members[4].objects
			for m, methods in members.members[4].methods.items():
				try:
					self.members.methods[m] += methods
				except KeyError:
					self.members.methods[m] = methods
		elif not self.members:
			self.members = members.members[4]

	def using(self, files, namespace, owner):
		if self.members:
			self.members.using(files, namespace, owner)

	def predecl(self, files, namespace, owner):
		if self.members:
			self.members.predecl(files, namespace, owner)

	def dump(self, files, namespace, owner):
		if self.members:
			self.members.dumpObjects(files, namespace, owner)
			self.members.dump(files, namespace, owner)
