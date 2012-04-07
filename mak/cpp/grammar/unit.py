import cpp.yacc as yacc
import rtti


class Expr(yacc.Nonterm):
	"%nonterm"

	def expr_namespace(self, namespace):
		"%reduce Namespace"
		self.value = namespace.value

	def expr_type(self, tags_left, cls, tags_right, semi):
		"%reduce TagsLeft TypeDef TagsRight SEMI"
		self.value = cls.value

	def expr_typedecl(self, tags_left, type, tags_right, semi):
		"%reduce TagsLeft TypeDecl TagsRight SEMI"
		self.value = None

	def expr_method(self, tags_left, method, tags_right, semi):
		"%reduce TagsLeft Method TagsRight SEMI"
		self.value = None

	def expr_method_definition(self, tags_left, method, tags_right, lbrace, skip_list, rbrace):
		"%reduce TagsLeft Method TagsRight LBRACE SkipList RBRACE"
		self.value = None

	def expr_variable(self, tags_left, variable, tags_right, semi):
		"%reduce TagsLeft Variable TagsRight SEMI"
		self.value = None

	def expr_using(self, using):
		"%reduce Using"
		self.value = None

	def expr_semi(self, semi):
		"%reduce SEMI"
		self.value = None




class ExprTemplate(yacc.Nonterm):
	"%nonterm"

	def expr(self, expr):
		"%reduce Expr"
		self.value = expr.value

	def expr_template(self, template, expr):
		"%reduce Template ExprTemplate"
		self.value = None




class Exprs(yacc.Nonterm):
	"%nonterm"

	def expr_list(self, expr, exprs):
		"%reduce ExprTemplate Exprs"
		if expr.value:
			self.values = [expr.value]+exprs.values
		else:
			self.values = exprs.values[:]

	def empty(self):
		"%reduce"
		self.values = []




class Unit(yacc.Nonterm):
	"%start"

	def __init__(self, parser):
		yacc.Nonterm.__init__(self, parser)
		self.result = parser.root

	def exprs(self, exprs):
		"%reduce Exprs"
		for obj in exprs.values:
			self.result.addObject(obj)

