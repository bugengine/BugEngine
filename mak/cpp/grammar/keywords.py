import cpp.yacc as yacc


class Operator(yacc.Nonterm):
	"%nonterm"
	def op_new(self, op):
		"%reduce NEW"
		self.value = op.value
	def op_delete(self, op):
		"%reduce DELETE"
		self.value = op.value
	def op_times(self, op):
		"%reduce TIMES"
		self.value = op.value
	def op_divide(self, op):
		"%reduce DIVIDE"
		self.value = op.value
	def op_mod(self, op):
		"%reduce MOD"
		self.value = op.value
	def op_plus(self, op):
		"%reduce PLUS"
		self.value = op.value
	def op_minus(self, op):
		"%reduce MINUS"
		self.value = op.value
	def op_lshift(self, op):
		"%reduce LSHIFT"
		self.value = op.value
	def op_rshift(self, op):
		"%reduce RSHIFT"
		self.value = op.value
	def op_and(self, op):
		"%reduce AND"
		self.value = op.value
	def op_or(self, op):
		"%reduce OR"
		self.value = op.value
	def op_xor(self, op):
		"%reduce XOR"
		self.value = op.value
	def op_not(self, op):
		"%reduce NOT"
		self.value = op.value
	def op_land(self, op):
		"%reduce LAND"
		self.value = op.value
	def op_lor(self, op):
		"%reduce LOR"
		self.value = op.value
	def op_lnot(self, op):
		"%reduce LNOT"
		self.value = op.value
	def op_le(self, op):
		"%reduce LE"
		self.value = op.value
	def op_ge(self, op):
		"%reduce GE"
		self.value = op.value
	def op_eq(self, op):
		"%reduce EQ"
		self.value = op.value
	def op_ne(self, op):
		"%reduce NE"
		self.value = op.value
	def op_equal(self, op):
		"%reduce EQUAL"
		self.value = op.value
	def op_timesequal(self, op):
		"%reduce TIMESEQUAL"
		self.value = op.value
	def op_divequal(self, op):
		"%reduce DIVEQUAL"
		self.value = op.value
	def op_modequal(self, op):
		"%reduce MODEQUAL"
		self.value = op.value
	def op_plusequal(self, op):
		"%reduce PLUSEQUAL"
		self.value = op.value
	def op_minusequal(self, op):
		"%reduce MINUSEQUAL"
		self.value1 = op.value
	def op_lshiftequal(self, op):
		"%reduce LSHIFTEQUAL"
		self.value = op.value
	def op_rshiftequal(self, op):
		"%reduce RSHIFTEQUAL"
		self.value = op.value
	def op_andequal(self, op):
		"%reduce ANDEQUAL"
		self.value = op.value
	def op_orequal(self, op):
		"%reduce OREQUAL"
		self.value = op.value
	def op_xorequal(self, op):
		"%reduce XOREQUAL"
		self.value = op.value
	def op_plusplus(self, op):
		"%reduce PLUSPLUS"
		self.value = op.value
	def op_minusminus(self, op):
		"%reduce MINUSMINUS"
		self.value = op.value
	def op_arrow(self, op):
		"%reduce ARROW"
		self.value = op.value
	def op_condop(self, op):
		"%reduce CONDOP"
		self.value = op.value
	def op_period(self, op):
		"%reduce PERIOD"
		self.value = op.value
	def op_colon(self, op):
		"%reduce COLON"
		self.value = op.value
	def op_ellipsis(self, op):
		"%reduce ELLIPSIS"
		self.value = op.value



class Keyword(yacc.Nonterm):
	"%nonterm"
	def kw_struct(self, kw):
		"%reduce STRUCT"
		self.value = kw.value
	def kw_class(self, kw):
		"%reduce CLASS"
		self.value = kw.value
	def kw_enum(self, kw):
		"%reduce ENUM"
		self.value = kw.value
	def kw_namespace(self, kw):
		"%reduce NAMESPACE"
		self.value = kw.value
	def kw_union(self, kw):
		"%reduce UNION"
		self.value = kw.value
	def kw_using(self, kw):
		"%reduce USING"
		self.value = kw.value
	def kw_public(self, kw):
		"%reduce PUBLIC"
		self.value = kw.value
	def kw_protected(self, kw):
		"%reduce PROTECTED"
		self.value = kw.value
	def kw_private(self, kw):
		"%reduce PRIVATE"
		self.value = kw.value
	def kw_friend(self, kw):
		"%reduce FRIEND"
		self.value = kw.value
	def kw_signed(self, kw):
		"%reduce SIGNED"
		self.value = kw.value
	def kw_unsigned(self, kw):
		"%reduce UNSIGNED"
		self.value = kw.value
	def kw_char(self, kw):
		"%reduce CHAR"
		self.value = kw.value
	def kw_short(self, kw):
		"%reduce SHORT"
		self.value = kw.value
	def kw_long(self, kw):
		"%reduce LONG"
		self.value = kw.value
	def kw_int(self, kw):
		"%reduce INT"
		self.value = kw.value
	def kw_float(self, kw):
		"%reduce FLOAT"
		self.value = kw.value
	def kw_double(self, kw):
		"%reduce DOUBLE"
		self.value = kw.value
	def kw_explicit(self, kw):
		"%reduce EXPLICIT"
		self.value = kw.value
	def kw_extern(self, kw):
		"%reduce EXTERN"
		self.value = kw.value
	def kw_inline(self, kw):
		"%reduce INLINE"
		self.value = kw.value
	def kw_static(self, kw):
		"%reduce STATIC"
		self.value = kw.value
	def kw_const(self, kw):
		"%reduce CONST"
		self.value = kw.value
	def kw_volatile(self, kw):
		"%reduce VOLATILE"
		self.value = kw.value
	def kw_virtual(self, kw):
		"%reduce VIRTUAL"
		self.value = kw.value
	def kw_override(self, kw):
		"%reduce OVERRIDE"
		self.value = kw.value
	def kw_template(self, kw):
		"%reduce TEMPLATE"
		self.value = kw.value
	def kw_typename(self, kw):
		"%reduce TYPENAME"
		self.value = kw.value
	def kw_operator(self, kw):
		"%reduce OPERATOR"
		self.value = kw.value
	def kw_typedef(self, kw):
		"%reduce TYPEDEF"
		self.value = kw.value
	def kw_throw(self, kw):
		"%reduce THROW"
		self.value = kw.value
	def kw_be_tag(self, kw):
		"%reduce BE_TAG"
		self.value = kw.value