import cpp




class Arg(cpp.yacc.Nonterm):
	"%nonterm"

	def arg_value(self, tags_left, type, id, equal, value, tags_right):
		"%reduce TagsLeft Type ID EQUAL Value TagsRight"
		self.type = type.value
		self.name = id.value

	def arg_full(self, tags_left, type, id, tags_right):
		"%reduce TagsLeft Type ID TagsRight"
		self.type = type.value
		self.name = id.value

	def arg_unnamed(self, tags_left, type, tags_right):
		"%reduce TagsLeft Type TagsRight"
		self.type = type.value
		self.name = ""

	def arg_ellipsis(self, ellipsis):
		"%reduce ELLIPSIS"




class ArgSequence(cpp.yacc.Nonterm):
	"%nonterm"

	def argseq(self, argseq, comma, tags, arg):
		"%reduce ArgSequence COMMA TagsRight Arg"
		self.args = argseq.args
		self.args.append(arg)

	def argseq_1arg(self, arg):
		"%reduce Arg"
		self.args = [arg]




class ArgList(cpp.yacc.Nonterm):
	"%nonterm"

	def no_args(self):
		"%reduce"
		self.args = []

	def arglist(self, arg_sequence):
		"%reduce ArgSequence"
		self.args = arg_sequence.args

	def dump(self, file, instances, name, owner, parent_name, decl):
		arg_pointer = "0"
		arg_index = 0
		for arg in self.args[::-1]:
			arg_tag = "0"
			file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter s_%s_p%d =\n" % (decl, arg_index))
			file.write("    {\n")
			file.write("        {%s},\n" % arg_tag)
			file.write("        {%s},\n" % arg_pointer)
			file.write("        \"%s\",\n" % arg.name)
			file.write("        ::BugEngine::be_typeid< %s >::type()\n" % arg.type)
			file.write("    };\n")
			arg_pointer = "&s_%s_p%d" % (decl, arg_index)
			arg_index = arg_index + 1





class MethodPrototype(cpp.yacc.Nonterm):
	"%nonterm"

	def method_simple(self, type, name, lparen, args, rparen):
		"%reduce Type Name LPAREN ArgList RPAREN"
		self.id = name.value
		self.name = name.value
		self.return_type = type.value
		self.line = name.lineno
		self.args = args
		self.attributes = set()

	def method_operator(self, type, keyword, operator, lparen, args, rparen):
		"%reduce Type OPERATOR Operator LPAREN ArgList RPAREN"
		self.id = 'operator%s'%operator.value
		self.name = operator.rtti_name
		self.return_type = type.value
		self.line = operator.lineno
		self.args = args
		self.attributes = set()

	def method_constructor(self, type, lparen, args, rparen):
		"%reduce Type LPAREN ArgList RPAREN"
		self.id = '?new'
		self.name = '?new'
		self.return_type = type.value
		self.line = type.lineno
		self.args = args
		self.attributes = set(['static'])

	def method_destructor(self, n, type, lparen, args, rparen):
		"%reduce NOT Type LPAREN ArgList RPAREN"
		self.id = '?del'
		self.name = '?del'
		self.return_type = type.value
		self.line = type.lineno
		self.args = args
		self.attributes = set(['static'])




class MethodAttributes(cpp.yacc.Nonterm):
	"%nonterm"

	def method(self, method):
		"%reduce MethodPrototype"
		self.value = method

	def method_static(self, static, method):
		"%reduce STATIC MethodAttributes"
		self.value = method.value
		self.value.attributes.add('static')

	def method_virtual(self, virtual, method):
		"%reduce VIRTUAL MethodAttributes"
		self.value = method.value
		self.value.attributes.add('virtual')

	def method_inline(self, inline, method):
		"%reduce INLINE MethodAttributes"
		self.value = method.value
		self.value.attributes.add('inline')




class Initializers(cpp.yacc.Nonterm):
	"%nonterm"

	def initializers(self, name, lparen, value, rparen):
		"%reduce ID LPAREN Value RPAREN"
	def initializer_list(self, name, lparen, value, rparen, comma, initlist):
		"%reduce ID LPAREN Value RPAREN COMMA Initializers"




class Method(cpp.yacc.Nonterm):
	"%nonterm"

	def method(self, method):
		"%reduce MethodAttributes"
		self.value = method.value

	def method_const(self, method, const):
		"%reduce Method CONST"
		self.value = method.value
		self.value.attributes.add('const')

	def method_override(self, method, override):
		"%reduce Method OVERRIDE"
		self.value = method.value
		self.value.attributes.add('override')

	def method_abstract(self, method, equals, value):
		"%reduce Method EQUAL Constant"
		self.value = method.value
		self.value.attributes.add('abstract')

	def method_initlist(self, method, colon, initializers):
		"%reduce Method COLON Initializers"
		self.value = method.value

	def dump(self, file, instances, name, owner, parent_name, parent_value, overload_ptr, overload_index):
		tags = "0"

		fullname = '::'+'::'.join(name)
		decl = fullname.replace(':', '_')
		prettyname = self.value.name.replace("?", "_")
		new_overload = "&s_%s_%s_%d" % (decl, prettyname, overload_index)
		#args,param_types,return_type = self.value.args.dump(file, instances, name, owner, member, not 'static' in self.value.attributes)
		args = "0"
		param_types = ""
		return_type = ""
		method_tags = "0"

		if self.value.id == '?del':
			return overload_index
		else:
			if self.value.id == '?new':
				if param_types: param_types = ', '+param_types
				helper = "BugEngine::RTTI::procedurehelper< %s%s >" % (parent_name, param_types)
				if parent_value:
					call_ptr = "&%s::construct" % helper
					return_type = parent_name
				else:
					call_ptr = "&%s::constructPtr" % helper
					return_type = "ref< %s >" % parent_name
			else:
				ptr = "%s (*) (%s)" % (return_type, param_types)
				if 'const' in self.value.attributes:
					ptr += ' const'
				methodptr = "BE_SELECTOVERLOAD(%s)&%s::%s" % (ptr, name, self.value.name)
				if param_types: param_types = ', '+param_types
				if return_type != 'void':
					helper = "BugEngine::RTTI::functionhelper< %s, %s%s >" % ("::BugEngine::RTTI::Class", return_type, param_types)
				else:
					helper = "BugEngine::RTTI::procedurehelper< %s%s >" % ("::BugEngine::RTTI::Class", param_types)
				if 'static' in self.value.attributes or not parent_name:
					call_ptr = "&%s::callStatic< %s >" % (helper, methodptr)
				else:
					call_ptr = "&%s::call< %s >" % (helper, methodptr)

			file.write("static const ::BugEngine::RTTI::Method::Overload %s =\n" % (new_overload))
			file.write("    {\n")
			file.write("        {%s},\n" % method_tags)
			file.write("        {%s},\n" % overload_ptr)
			file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % return_type)
			file.write("        {%s},\n" % args)
			file.write("        %s::VarArg,\n" % helper)
			file.write("        %s\n" % call_ptr)
			file.write("    };\n")
			return new_overload



class MethodPointer(cpp.yacc.Nonterm):
	"%nonterm"

	def method_ptr(self, type, lparen, times, rparen, lparen2, args, rparen2):
		"%reduce Type LPAREN TIMES RPAREN LPAREN ArgList RPAREN"
	def method_ptr(self, type, lparen, times, id, rparen, lparen2, args, rparen2):
		"%reduce Type LPAREN TIMES ID RPAREN LPAREN ArgList RPAREN"

