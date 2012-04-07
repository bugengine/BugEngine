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
	def arglist(self, argsequence):
		"%reduce ArgSequence"




class MethodPrototype(yacc.Nonterm):
	"%nonterm"

	def method_simple(self, type, name, lparen, args, rparen):
		"%reduce Type ID LPAREN ArgList RPAREN"
		self.value = None

	def method_operator(self, type, keyword, operator, lparen, args, rparen):
		"%reduce Type OPERATOR Operator LPAREN ArgList RPAREN"
		self.value = None

	def method_constructor(self, type, lparen, args, rparen):
		"%reduce Type LPAREN ArgList RPAREN"
		self.value = None

	def method_destructor(self, type, lparen, args, rparen):
		"%reduce NOT Type LPAREN ArgList RPAREN"
		self.value = None




class MethodAttributes(yacc.Nonterm):
	"%nonterm"

	def method(self, method):
		"%reduce MethodPrototype"

	def method_static(self, static, method):
		"%reduce STATIC MethodAttributes"
		self.value = None

	def method_virtual(self, virtual, method):
		"%reduce VIRTUAL MethodAttributes"
		self.value = None

	def method_inline(self, inline, method):
		"%reduce INLINE MethodAttributes"
		self.value = None




class Method(yacc.Nonterm):
	"%nonterm"

	def method(self, method):
		"%reduce MethodAttributes"
		self.value = None

	def method_const(self, method, const):
		"%reduce Method CONST"
		self.value = None

	def method_override(self, method, override):
		"%reduce Method OVERRIDE"
		self.value = None

	def method_abstract(self, method, equals, value):
		"%reduce Method EQUAL Value"
		self.value = None
