import cpp


class ExprMethod(cpp.yacc.Nonterm):
	"%nonterm"

	def expr_method(self, tags_left, method, tags_right, semi):
		"%reduce TagsLeft Method TagsRight SEMI"
		self.value = method
		self.value.tags = tags_left.tags + tags_right.tags

	def expr_method_definition(self, tags_left, method, tags_right, lbrace, skip_list, rbrace):
		"%reduce TagsLeft Method TagsRight LBRACE SkipList RBRACE"
		self.value = method
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
			self.methods[m.value.value.name].append(m.value)
		except KeyError:
			self.methods[m.value.value.name] = [m.value]

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

	def predecl(self, file, instances, name, parent_name):
		for o in self.objects:
			o.predecl(file, instances, name, parent_name)

	def dump(self, file, instances, namespace, name, parent_name, parent_value):
		if parent_name:
			method_ptr = "be_typeid< %s >::klass()->methods"%parent_name
			property_ptr = "be_typeid< %s >::klass()->properties"%parent_name
			object_ptr = "be_typeid< %s >::klass()->objects"%parent_name
		else:
			method_ptr = "{0}"
			property_ptr = "{0}"
			object_ptr = "{0}"
		constructor_ptr = "{0}"
		call_ptr = "{0}"

		for o in self.objects:
			object_ptr = o.dump(file, instances, namespace, name, parent_name, object_ptr)

		for m, overloads in self.methods.items():
			overload_ptr = "0"
			overload_index = 0
			for overload in overloads:
				overload_ptr = overload.dump(file, instances, namespace, name, parent_name, parent_value, overload_ptr, overload_index)
				overload_index += 1

			decl = '_'.join(name)
			prettyname = m.replace("?", "_")

			file.write("static const ::BugEngine::RTTI::Method s_method_%s_%s =\n" % (decl, prettyname))
			file.write("    {\n")
			file.write("        \"%s\",\n" % m)
			file.write("        {&s_method_%s_%s},\n" % (decl, prettyname))
			file.write("        %s,\n" % method_ptr)
			file.write("        {%s}\n" % overload_ptr)
			file.write("    };\n")
			if m == "?ctor":
				constructor_ptr = "{&s_method_%s_%s}" % (decl, prettyname)
			elif m == "?call":
				call_ptr = "{&s_method_%s_%s}" % (decl, prettyname)
			method_ptr = "{&s_method_%s_%s}" % (decl, prettyname)
		return object_ptr, method_ptr, constructor_ptr, call_ptr


