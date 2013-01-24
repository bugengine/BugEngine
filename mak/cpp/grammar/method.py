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

	def dump(self, files, namespace, parent, decl, static, const):
		arg_pointer = "{0}"
		arg_index = 0
		for arg in self.args[::-1]:
			arg_tag = "{0}" #arg.tags.dump(files, decl)
			files[0].write("static const ::BugEngine::RTTI::Method::Overload::Parameter %s_param_%d =\n" % (decl, arg_index))
			files[0].write("{\n")
			files[0].write("    %s,\n" % arg_tag)
			files[0].write("    %s,\n" % arg_pointer)
			files[0].write("    \"%s\",\n" % arg.name)
			files[0].write("    ::BugEngine::be_typeid< %s >::type()\n" % arg.type)
			files[0].write("};\n")
			arg_pointer = "{&%s_param_%d}" % (decl, arg_index)
			arg_index = arg_index + 1
		if parent and not static:
			arg_tag = "{0}"
			files[0].write("static const ::BugEngine::RTTI::Method::Overload::Parameter %s_param_%d =\n" % (decl, arg_index))
			files[0].write("{\n")
			files[0].write("    %s,\n" % arg_tag)
			files[0].write("    %s,\n" % arg_pointer)
			files[0].write("    \"%s\",\n" % "this")
			if const:
				files[0].write("    ::BugEngine::be_typeid< const %s& >::type()\n" % '::'.join(parent))
			else:
				files[0].write("    ::BugEngine::be_typeid< %s& >::type()\n" % '::'.join(parent))
			files[0].write("};\n")
			arg_pointer = "{&%s_param_%d}" % (decl, arg_index)
		return arg_pointer





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



template_header = """#line %(LINE)d
static ::BugEngine::RTTI::Value %(PREFIX)s_trampoline(::BugEngine::RTTI::Value* params, u32 paramCount)
#line %(LINE)d
{
#line %(LINE)d
    be_forceuse(paramCount);
#line %(LINE)d
    be_forceuse(params);
#line %(LINE)d
    be_assert(paramCount == %(PARAMCOUNT)d, "%%s: expected %%d parameters, got %%d" | "%(ID)s" | paramCount | %(PARAMCOUNT)d);
"""
template_footer = """#line %(LINE)d
}
"""

template_class_procedure = """
#line %(LINE)d
	%(ID)s(%(PARAMS)s);
#line %(LINE)d
	return ::BugEngine::RTTI::Value();
"""
template_member_procedure = """
#line %(LINE)d
	params[0].as<%(CLASS)s&>().%(ID)s(%(PARAMS)s);
#line %(LINE)d
	return ::BugEngine::RTTI::Value();
"""

template_class_function = """
#line %(LINE)d
	return ::BugEngine::RTTI::Value(%(ID)s(%(PARAMS)s));
"""

template_member_function = """
#line %(LINE)d
	return ::BugEngine::RTTI::Value(params[0].as<%(CLASS)s&>().%(ID)s(%(PARAMS)s));
"""
template_new_ptr = """
#line %(LINE)d
	::BugEngine::RTTI::Value v (::BugEngine::be_typeid< minitl::ref<%(CLASS)s > >::type(), ::BugEngine::RTTI::Value::Reserve);
	new(v.memory()) minitl::ref< %(CLASS)s >( minitl::ref< %(CLASS)s >::create(::BugEngine::Arena::script()%(PARAMSCOMMA)s) );
	return v;
"""
template_new_value = """
#line %(LINE)d
	::BugEngine::RTTI::Value v (::BugEngine::be_typeid< %(CLASS)s >::type(), ::BugEngine::RTTI::Value::Reserve);
	new(v.memory()) %(CLASS)s(%(PARAMS)s);
	return v;
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

	def predecl(self, files, namespace, parent, parent_value, pretty_name, index):
		funname = '%s_%d' % (pretty_name, index)
		if self.value.name == '?new':
			if parent_value:
				self.trampoline = self.dump_new_ref(files, namespace, parent, funname)
			else:
				self.trampoline = self.dump_new_struct(files, namespace, parent, funname)
		elif not parent or 'static' in self.value.attributes:
			if self.value.return_type != 'void':
				self.trampoline = self.dump_static_method(files, namespace, parent, funname)
			else:
				self.trampoline = self.dump_static_procedure(files, namespace, parent, funname)
		else:
			if self.value.return_type != 'void':
				self.trampoline = self.dump_member_method(files, namespace, parent, funname)
			else:
				self.trampoline = self.dump_member_procedure(files, namespace, parent, funname)

	def dump_new_ref(self, files, namespace, parent, funname):
		return "0"

	def dump_new_struct(self, files, namespace, parent, funname):
		return "0"

	def dump_static_method(self, files, namespace, parent, funname):
		return "0"

	def dump_static_procedure(self, files, namespace, parent, funname):
		return "0"

	def dump_member_method(self, files, namespace, parent, funname):
		return "0"

	def dump_member_procedure(self, files, namespace, parent, funname):
		return "0"

	def dump(self, files, namespace, parent, name, previous, index):
		method_name = '%s_overload_%d' % (name, index)
		arguments = self.value.args.dump(files, namespace, parent, method_name, 'static' in self.value.attributes, 'const' in self.value.attributes)
		files[0].write('static const ::BugEngine::RTTI::Method::Overload %s =\n' % method_name)
		files[0].write('{\n')
		files[0].write('	{0},\n')
		files[0].write('	%s,\n' % previous)
		files[0].write('	::BugEngine::be_typeid< %s >::type(),\n' % self.value.return_type)
		files[0].write('	%s,\n' % arguments)
		files[0].write('	0,\n')
		files[0].write('	%s\n' % self.trampoline)
		files[0].write('};\n')
		return '{&%s}'% (method_name)



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
