import cpp


class NameItem(cpp.yacc.Nonterm):
	"%nonterm"
	def name_id(self, id):
		"%reduce ID"
		self.value = id.value
		self.lineno = id.lineno
	#def name_template_class(self, template, id):
	#	"%reduce TEMPLATE ID"
	#	self.value = template.value + " " + id.value
	#	self.lineno = id.lineno
	def name_typename(self, typename, id):
		"%reduce TYPENAME ID"
		self.value = typename.value + " " + id.value
		self.lineno = id.lineno
	def name_template(self, id, lt, skip, gt):
		"%reduce ID LT SkipListTemplate GT"
		self.value = id.value + " < " + skip.value + " >"
		self.lineno = id.lineno

class Namelist(cpp.yacc.Nonterm):
	"%nonterm"
	def nameitem(self, name_item):
		"%reduce NameItem"
		self.value = name_item.value
		self.lineno = name_item.lineno
	def namelist(self, name_list, scope, name_item):
		"%reduce Namelist SCOPE NameItem"
		self.value = name_list.value + '::' + name_item.value
		self.lineno = name_list.lineno

class NamePrecedence(cpp.yacc.Precedence):
	"%right <ScopePrecedence"
class Name(cpp.yacc.Nonterm):
	"%nonterm"
	def name_absolute(self, scope, name_list):
		"%reduce SCOPE Namelist [NamePrecedence]"
		self.value = '::'+name_list.value
		self.lineno = scope.lineno
	def name(self, name_list):
		"%reduce Namelist [NamePrecedence]"
		self.value = name_list.value
		self.lineno = name_list.lineno

class NameOpt(cpp.yacc.Nonterm):
	"%nonterm"
	def name(self, name):
		"%reduce Name"
		self.value = name.value
		self.lineno = name.lineno

	def empty(self):
		"%reduce"
		self.value = ""
		self.lineno = 0
