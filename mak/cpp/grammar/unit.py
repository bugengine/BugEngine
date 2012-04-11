import cpp


class ExprMethod(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_method(self, tags_left, method, tags_right, semi):
		"%reduce TagsLeft Method TagsRight SEMI"
		self.value = method.value
		self.value.tags = tags_left.tags + tags_right.tags

	def expr_method_definition(self, tags_left, method, tags_right, lbrace, skip_list, rbrace):
		"%reduce TagsLeft Method TagsRight LBRACE SkipList RBRACE"
		self.value = method.value
		self.value.tags = tags_left.tags + tags_right.tags




class ExprNamespace(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_namespace(self, namespace):
		"%reduce Namespace"
		self.value = namespace




class ExprType(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_type(self, tags_left, type, tags_right, semi):
		"%reduce TagsLeft TypeDef TagsRight SEMI"
		self.value = type.value
		#self.value.tags = tags_left.tags + tags_right.tags

	def expr_typedecl(self, tags_left, type, tags_right, semi):
		"%reduce TagsLeft TypeDecl TagsRight SEMI"
		self.value = None
		#self.value.tags = tags_left.tags + tags_right.tags

	def expr_typedef(self, tags_left, typedef, typedecl, id, tags_right, semi):
		"%reduce TagsLeft TYPEDEF TypeDecl ID TagsRight SEMI"
		self.value = None

	def expr_typedef_method(self, tags_left, typedef, methodptr, tags_right, semi):
		"%reduce TagsLeft TYPEDEF MethodPointer TagsRight SEMI"
		self.value = None




class ExprVariable(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_variable(self, tags_left, variable, tags_right, semi):
		"%reduce TagsLeft Variable TagsRight SEMI"
		self.value = None
		#self.value.tags = tags_left.tags + tags_right.tags


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
		try:
			self.methods[m.value.name].append(m.value)
		except KeyError:
			self.methods[m.value.name] = [m.value]

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
		#if t.value:
		#	self.members.append(t.value)

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

	def predecl(self, file, instances, name, parent, member):
		for o in self.objects:
			o.predecl(file, instances, name, parent, member)

	def dump(self, file, instances, name, parent, member):
		for o in self.objects:
			o.dump(file, instances, name, parent, member)




class Unit(cpp.yacc.Nonterm):
	"%start"

	def __init__(self, parser):
		cpp.yacc.Nonterm.__init__(self, parser)
		parser.root = self

	def exprs(self, exprs):
		"%reduce Exprs"
		self.members = exprs

	def dump(self, file, instances):
		instances.write("#include    <rtti/stdafx.h>\n")
		instances.write("#include    <%s>\n" % self.parser.source)
		instances.write("#include    <rtti/typeinfo.hh>\n")
		instances.write("#include    <rtti/classinfo.script.hh>\n")

		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % self.parser.source)
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/typeinfo.inl>\n")
		file.write("#include    <rtti/value.hh>\n")
		file.write("#include    <rtti/value.inl>\n")
		file.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/taginfo.script.hh>\n")
		file.write("#include    <rtti/engine/helper/get.hh>\n")
		file.write("#include    <rtti/engine/helper/set.hh>\n")
		file.write("#include    <rtti/engine/helper/method.hh>\n")
		file.write("#include    <rtti/tags/documentation.script.hh>\n")
		file.write("\n")

		owner = "be_%s_Namespace()"%self.parser.plugin

		file.write("namespace BugEngine\n{\n\n")
		file.write("raw< ::BugEngine::RTTI::Class > %s;\n"%owner)
		self.members.predecl(file, instances, [], owner, False)
		file.write("\n}\n\n")

		instances.write("namespace BugEngine\n{\n\n")
		self.members.dump(file, instances, [], owner, False)
		instances.write("\n}\n\n")





