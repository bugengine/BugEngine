import cpp.yacc as yacc




class Arg(yacc.Nonterm):
	"%nonterm"

	def arg_full(self, tags_left, type, id, tags_right):
		"%reduce TagsLeft Type ID TagsRight"

	def arg_unnamed(self, tags_left, type, tags_right):
		"%reduce TagsLeft Type TagsRight"

	def arg_ellipsis(self, ellipsis):
		"%reduce ELLIPSIS"




class ArgSequence(yacc.Nonterm):
	"%nonterm"

	def argseq(self, argseq, comma, arg):
		"%reduce ArgSequence COMMA Arg"

	def argseq_1arg(self, arg):
		"%reduce Arg"




class ArgList(yacc.Nonterm):
	"%nonterm"

	def no_args(self):
		"%reduce"
		self.args = None

	def arglist(self, argsequence):
		"%reduce ArgSequence"
		self.args = None




class MethodPrototype(yacc.Nonterm):
	"%nonterm"

	def method_simple(self, type, name, lparen, args, rparen):
		"%reduce Type ID LPAREN ArgList RPAREN"
		self.name = name.value
		self.line = name.lineno
		self.args = args
		self.attributes = set()

	def method_operator(self, type, keyword, operator, lparen, args, rparen):
		"%reduce Type OPERATOR Operator LPAREN ArgList RPAREN"
		self.name = operator.rtti_name
		self.line = operator.lineno
		self.args = args
		self.attributes = set()

	def method_constructor(self, type, lparen, args, rparen):
		"%reduce Type LPAREN ArgList RPAREN"
		self.name = '?new'
		self.line = type.lineno
		self.args = args
		self.attributes = set(['static'])

	def method_destructor(self, type, lparen, args, rparen):
		"%reduce NOT Type LPAREN ArgList RPAREN"
		self.name = '?del'
		self.line = name.lineno
		self.args = args
		self.attributes = set(['static'])




class MethodAttributes(yacc.Nonterm):
	"%nonterm"

	def method(self, method):
		"%reduce MethodPrototype"
		self.value = method

	def method_static(self, static, method):
		"%reduce STATIC MethodAttributes"
		self.value = method
		self.value.attributes.add('static')

	def method_virtual(self, virtual, method):
		"%reduce VIRTUAL MethodAttributes"
		self.value = method
		self.value.attributes.add('virtual')

	def method_inline(self, inline, method):
		"%reduce INLINE MethodAttributes"
		self.value = method
		self.value.attributes.add('inline')




class Method(yacc.Nonterm):
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
		"%reduce Method EQUAL Value"
		self.value = method.value
		self.value.attributes.add('abstract')

