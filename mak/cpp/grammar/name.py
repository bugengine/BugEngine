import cpp.yacc as yacc

class NameItem(yacc.Nonterm):
	"%nonterm"
	def name_id(self, id):
		"%reduce ID"
		self.value = id.value
	#def name_template_class(self, template, id):
	#	"%reduce TEMPLATE ID"
	#	self.value = template.value + " " + id.value
	def name_typename(self, typename, id):
		"%reduce TYPENAME ID"
		self.value = typename.value + " " + id.value
	def name_template(self, id, lt, skip, gt):
		"%reduce ID LT SkipListTemplate GT"
		self.value = id.value + " < " + skip.value + " >"

class Namelist(yacc.Nonterm):
	"%nonterm"
	def nameitem(self, name_item):
		"%reduce NameItem"
		self.value = name_item.value
	def namelist(self, name_list, scope, name_item):
		"%reduce Namelist SCOPE NameItem"
		self.value = name_list.value + '::' + name_item.value

class Name(yacc.Nonterm):
	"%nonterm"
	def name_absolute(self, scope, name_list):
		"%reduce SCOPE Namelist"
		self.value = '::'+name_list.value
	def name(self, name_list):
		"%reduce Namelist"
		self.value = name_list.value


