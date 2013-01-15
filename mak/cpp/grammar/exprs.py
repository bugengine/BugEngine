import cpp


class ExprMethod(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_method(self, tags_left, method, tags_right, semi):
		"%reduce TagsLeft Method TagsRight SEMI"
		self.value = method
		self.value.tags = tags_left
		self.value.tags.tags += tags_right.tags
		self.value.tags.comment_right = tags_right.comment
		self.value.aliases = tags_left.aliases


	def expr_method_definition(self, tags_left, method, tags_right, lbrace, skip_list, rbrace):
		"%reduce TagsLeft Method TagsRight LBRACE SkipList RBRACE"
		self.value = method
		self.value.tags = tags_left
		self.value.tags.tags += tags_right.tags
		self.value.tags.comment_right = tags_right.comment
		self.value.aliases = tags_left.aliases





class ExprNamespace(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_namespace(self, namespace):
		"%reduce Namespace"
		self.value = namespace


class TypeDef:
	def __init__(self, name):
		self.name = name

	def dump(self, file, instances, namespace, name, member, object_ptr):
		if member:
			file.write("typedef %s::%s %s;\n" % ('::'.join(name), self.name, self.name))


class ExprType(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_type(self, tags_left, type, tags_right, semi):
		"%reduce TagsLeft TypeDef TagsRight SEMI"
		self.value = type.value
		self.value.tags = tags_left
		self.value.tags.tags += tags_right.tags
		self.value.tags.comment_right = tags_right.comment
		self.value.aliases = tags_left.aliases

	def expr_typedecl(self, tags_left, type, tags_right, semi):
		"%reduce TagsLeft TypeDecl TagsRight SEMI"
		self.value = None

	def expr_typedef(self, tags_left, typedef, typedecl, id, tags_right, semi):
		"%reduce TagsLeft TYPEDEF Type ID TagsRight SEMI"
		self.value = TypeDef(id.value)

	def expr_typedef_array(self, tags_left, typedef, typedecl, id, lbracket, value, rbracket, tags_right, semi):
		"%reduce TagsLeft TYPEDEF Type ID LBRACKET Value RBRACKET TagsRight SEMI"
		self.value = TypeDef(id.value)

	def expr_typedef_method(self, tags_left, typedef, method_ptr, tags_right, semi):
		"%reduce TagsLeft TYPEDEF MethodPointer TagsRight SEMI"
		if method_ptr.name:
			self.value = TypeDef(method_ptr.name)
		else:
			self.value = None




class ExprVariable(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_variable(self, tags_left, variable, tags_right, semi):
		"%reduce TagsLeft Variable TagsRight SEMI"
		self.value = variable
		self.value.tags = tags_left
		self.value.tags.tags += tags_right.tags
		self.value.tags.comment_right = tags_right.comment
		self.value.aliases = tags_left.aliases


class ExprFriend(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_friend_type(self, friend, type, semi):
		"%reduce FRIEND Type SEMI"

	def expr_friend_method(self, friend, method, semi):
		"%reduce FRIEND Method SEMI"



class ExprTemplate(cpp.yacc.Nonterm):
	"%nonterm"
	def template_method(self, tpl, method):
		"%reduce Template ExprMethod"
	def template_friend(self, tpl, method):
		"%reduce Template ExprFriend"
	def template_type(self, tpl, type):
		"%reduce Template ExprType"
	def template_variable(self, tpl, var):
		"%reduce Template ExprVariable"
	def template_template(self, semi):
		"%reduce Template ExprTemplate"





class ExprIgnore(cpp.yacc.Nonterm):
	"%nonterm"
	def expr_using(self, using):
		"%reduce Using"
	def expr_semi(self, semi):
		"%reduce SEMI"




class Exprs(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_method(self, m, exprs):
		"%reduce ExprMethod Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects
		for name in m.value.aliases+[m.value.value.name]:
			try:
				self.methods[name].append(m.value)
			except KeyError:
				self.methods[name] = [m.value]

	def expr_namespace(self, n, exprs):
		"%reduce ExprNamespace Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects
		self.objects.append(n.value)

	def expr_type(self, t, exprs):
		"%reduce ExprType Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects
		if t.value:
			self.objects.append(t.value)

	def expr_variable(self, v, exprs):
		"%reduce ExprVariable Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects
		if v.value:
			self.members.append(v.value)

	def expr_template(self, e, exprs):
		"%reduce ExprTemplate Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects

	def expr_friend(self, f, exprs):
		"%reduce ExprFriend Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects

	def expr_ignore(self, i, exprs):
		"%reduce ExprIgnore Exprs"
		self.methods = exprs.methods
		self.members = exprs.members
		self.objects = exprs.objects

	def empty(self):
		"%reduce"
		self.methods = {}
		self.members = []
		self.objects = []

	def using(self, files, namespace, owner):
		for o in self.objects:
			o.using(files, namespace, owner)

	def predecl(self, files, namespace, owner):
		for o in self.objects:
			o.predecl(files, namespace, owner)

	def dumpObjects(self, files, namespace, owner):
		for o in self.objects:
			o.dump(files, namespace, owner)

	def dump(self, files, namespace, owner):
		for o in self.objects:
			o.dump(files, namespace, owner)


