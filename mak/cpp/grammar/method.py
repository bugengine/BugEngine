import cpp




class Arg(cpp.yacc.Nonterm):
	"%nonterm"

	def arg_value(self, tags_left, type, id, equal, value, tags_right):
		"%reduce TagsLeft Type ID EQUAL Value TagsRight"
		self.type = type.value
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags

	def arg_full(self, tags_left, type, id, tags_right):
		"%reduce TagsLeft Type ID TagsRight"
		self.type = type.value
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags

	def arg_unnamed(self, tags_left, type, tags_right):
		"%reduce TagsLeft Type TagsRight"
		self.type = type.value
		self.name = ""
		self.lineno = type.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags

	def arg_ellipsis(self, ellipsis):
		"%reduce ELLIPSIS"




class ArgSequence(cpp.yacc.Nonterm):
	"%nonterm"

	def argseq(self, argseq, comma, tags, arg):
		"%reduce ArgSequence COMMA TagsRight Arg"
		self.args = argseq.args
		self.args.append(arg)
		arg.tags.tags += tags.tags

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

	def dump(self, file, instances, decl, parent_name, is_static, is_const, lineno):
		arg_pointer = "0"
		arg_index = 0
		for arg in self.args[::-1]:
			arg_tag = arg.tags.dump(file, instances, decl+"_p%d"%arg_index)
			file.write("#line %d\n"%arg.lineno)
			file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter %s_p%d =\\\n" % (decl, arg_index))
			file.write("    {\\\n")
			file.write("        %s,\\\n" % arg_tag)
			file.write("        {%s},\\\n" % arg_pointer)
			file.write("        \"%s\",\\\n" % arg.name)
			file.write("        ::BugEngine::be_typeid< %s >::type()\\\n" % arg.type)
			file.write("    };\n")
			arg_pointer = "&%s_p%d" % (decl, arg_index)
			arg_index = arg_index + 1
		if parent_name and not is_static:
			arg_tag = "{0}"
			file.write("#line %d\n"%lineno)
			file.write("static const ::BugEngine::RTTI::Method::Overload::Parameter %s_p%d =\\\n" % (decl, arg_index))
			file.write("    {\\\n")
			file.write("        %s,\\\n" % arg_tag)
			file.write("        {%s},\\\n" % arg_pointer)
			file.write("        \"%s\",\\\n" % "this")
			if is_const:
				file.write("        ::BugEngine::be_typeid< const %s& >::type()\\\n" % parent_name)
				extra_args = ['const %s&' % parent_name]
			else:
				file.write("        ::BugEngine::be_typeid< %s& >::type()\\\n" % parent_name)
				extra_args = ['%s&' % parent_name]
			file.write("    };\n")
			arg_pointer = "&%s_p%d" % (decl, arg_index)
		else:
			extra_args = []
		return (arg_pointer, extra_args,[arg.type for arg in self.args])





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

	def method_operator_call(self, type, keyword, lparen_op, rparen_op, lparen, args, rparen):
		"%reduce Type OPERATOR LPAREN RPAREN LPAREN ArgList RPAREN"
		self.id = 'operator()'
		self.name = '?call'
		self.return_type = type.value
		self.line = lparen_op.lineno
		self.args = args
		self.attributes = set()

	def method_operator_index(self, type, keyword, lbracket, rbracket, lparen, args, rparen):
		"%reduce Type OPERATOR LBRACKET RBRACKET LPAREN ArgList RPAREN"
		self.id = 'operator[]'
		self.name = '?index'
		self.return_type = type.value
		self.line = lbracket.lineno
		self.args = args
		self.attributes = set()
		
	def method_operator_lt(self, type, keyword, lt, lparen, args, rparen):
		"%reduce Type OPERATOR LT LPAREN ArgList RPAREN"
		self.id = 'operator<'
		self.name = '?lt'
		self.return_type = type.value
		self.line = lt.lineno
		self.args = args
		self.attributes = set()

	def method_operator_gt(self, type, keyword, gt, lparen, args, rparen):
		"%reduce Type OPERATOR GT LPAREN ArgList RPAREN"
		self.id = 'operator>'
		self.name = '?gt'
		self.return_type = type.value
		self.line = gt.lineno
		self.args = args
		self.attributes = set()


	def method_operator_cast(self, keyword, type, lparen, args, rparen):
		"%reduce OPERATOR Type LPAREN ArgList RPAREN"
		self.id = 'operator %s'%type.value
		self.name = '#%s'%('_'.join(type.value.split()).replace('*', 'ptr').replace('&', 'ref'))
		self.return_type = type.value
		self.line = type.lineno
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


template_class_procedure = """#line %(LINE)d
static ::BugEngine::RTTI::Value %(PREFIX)s_trampoline(::BugEngine::RTTI::Value* params, u32 paramCount)
#line %(LINE)d
{
#line %(LINE)d
    be_forceuse(paramCount);
#line %(LINE)d
    be_forceuse(params);
#line %(LINE)d
    be_assert(paramCount == %(PARAMCOUNT)d, "%%s: expected %%d parameters, got %%d" | "%(ID)s" | paramCount | %(PARAMCOUNT)d);
#line %(LINE)d
	%(ID)s(%(PARAMS)s);
#line %(LINE)d
	return ::BugEngine::RTTI::Value();
#line %(LINE)d
}
"""
template_member_procedure = """#line %(LINE)d
static ::BugEngine::RTTI::Value %(PREFIX)s_trampoline(::BugEngine::RTTI::Value* params, u32 paramCount)
#line %(LINE)d
{
#line %(LINE)d
    be_forceuse(paramCount);
#line %(LINE)d
    be_forceuse(params);
#line %(LINE)d
    be_assert(paramCount == %(PARAMCOUNT)d, "%%s: expected %%d parameters, got %%d" | "%(ID)s" | paramCount | %(PARAMCOUNT)d);
#line %(LINE)d
	params[0].as<%(CLASS)s&>().%(ID)s(%(PARAMS)s);
#line %(LINE)d
	return ::BugEngine::RTTI::Value();
#line %(LINE)d
}
"""

template_class_function = """#line %(LINE)d
static ::BugEngine::RTTI::Value %(PREFIX)s_trampoline(::BugEngine::RTTI::Value* params, u32 paramCount)
#line %(LINE)d
{
#line %(LINE)d
    be_forceuse(paramCount);
#line %(LINE)d
    be_forceuse(params);
#line %(LINE)d
    be_assert(paramCount == %(PARAMCOUNT)d, "%%s: expected %%d parameters, got %%d" | "%(ID)s" | paramCount | %(PARAMCOUNT)d);
#line %(LINE)d
	::BugEngine::RTTI::Value(%(ID)s(%(PARAMS)s));
#line %(LINE)d
}
"""
template_member_function = """#line %(LINE)d
static ::BugEngine::RTTI::Value %(PREFIX)s_trampoline(::BugEngine::RTTI::Value* params, u32 paramCount)
#line %(LINE)d
{
#line %(LINE)d
    be_forceuse(paramCount);
#line %(LINE)d
    be_forceuse(params);
#line %(LINE)d
    be_assert(paramCount == %(PARAMCOUNT)d, "%%s: expected %%d parameters, got %%d" | "%(ID)s" | paramCount | %(PARAMCOUNT)d);
#line %(LINE)d
	return ::BugEngine::RTTI::Value(params[0].as<%(CLASS)s&>().%(ID)s(%(PARAMS)s));
#line %(LINE)d
}
"""


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

	def dump(self, file, instances, namespace, decl, name, parent_name, parent_value, overload_ptr, overload_index):
		if self.value.id == '?del':
			return overload_index
		elif self.value.id == '?new':
			return overload_index
		else:
			prettyname = self.value.name.replace("?", "_")
			prettyname = prettyname.replace("#", "_")
			decl = decl + [prettyname]
			fullname = '::'+'::'.join(name)
			prefix = '_'.join(decl)
			tags = self.tags.dump(file, instances, prefix)

			new_overload = "s_%s_%s_%d" % (prefix, prettyname, overload_index)
			args,extra_params,param_types = self.value.args.dump(
					file,
					instances,
					new_overload,
					parent_name,
					'static' in self.value.attributes,
					'const' in self.value.attributes,
					self.value.line)
			return_type = self.value.return_type
			if 'const' in self.value.attributes:
				constness = 'const '
			else:
				constness = ''

			vararg = 0
			method_ptr = "0"
			if return_type.strip() == 'Value':
				if len(param_types) == 2:
					if param_types[0].replace(' ', '') == 'Value*':
						if param_types[1].strip() == 'u32':
							vararg = 1
							if not parent_name:
								method_ptr = '&%s::%s'%(fullname, self.name)
							elif 'const' in self.value.attributes:
								method_ptr = '&BugEngine::RTTI::wrapCallConst< %s, &%s::%s >'%(fullname, fullname, self.value.id)
							else:
								method_ptr = '&BugEngine::RTTI::wrapCall< %s, &%s::%s >'%(fullname, fullname, self.value.id)
			else:
				param = 0
				file.write("#line %d\n"%self.value.line)
				if parent_name:
					param += 1
					if return_type == 'void':
						template = template_member_procedure
					else:
						template = template_member_function
				else:
					if return_type == 'void':
						template = template_class_procedure
					else:
						template = template_class_function
				method = template % {
						'PREFIX':new_overload,
						'PARAMCOUNT':len(extra_params)+len(param_types),
						'ID': self.value.id,
						'PARAMS':','.join(['params[%d].as< %s >()'%(i,s) for i,s in zip(xrange(param,param+len(param_types)), param_types)]),
						'CLASS':parent_name,
						'LINE':self.value.line
					}
				file.write(method)
				method_ptr = "&%s_trampoline"%new_overload
				
			

			file.write("#line %d\n"%self.value.line)
			file.write("static const ::BugEngine::RTTI::Method::Overload %s =\n" % (new_overload))
			file.write("#line %d\n"%self.value.line)
			file.write("    {\n")
			file.write("#line %d\n"%self.value.line)
			file.write("        %s,\n" % tags)
			file.write("#line %d\n"%self.value.line)
			file.write("        {%s},\n" % overload_ptr)
			file.write("#line %d\n"%self.value.line)
			file.write("        ::BugEngine::be_typeid< %s >::type(),\n" % return_type)
			file.write("#line %d\n"%self.value.line)
			file.write("        {%s},\n" % args)
			file.write("#line %d\n"%self.value.line)
			file.write("        %d,\n"%vararg)
			file.write("#line %d\n"%self.value.line)
			file.write("        %s\n" % method_ptr)
			file.write("#line %d\n"%self.value.line)
			file.write("    };\n\n")
			return "&"+new_overload



class MethodPointer(cpp.yacc.Nonterm):
	"%nonterm"

	def method_ptr(self, type, lparen, times, rparen, lparen2, arg_list, rparen2):
		"%reduce Type LPAREN TIMES RPAREN LPAREN ArgList RPAREN"
		self.type = type.value + '(*)('+','.join([arg.type for arg in arg_list.args])+')'
		self.name = ''
		self.lineno = times.lineno
	def method_ptr(self, type, lparen, times, id, rparen, lparen2, arg_list, rparen2):
		"%reduce Type LPAREN TIMES ID RPAREN LPAREN ArgList RPAREN"
		self.type = type.value + '(*)('+','.join([arg.type for arg in arg_list.args])+')'
		self.name = id.value
		self.lineno = id.lineno
