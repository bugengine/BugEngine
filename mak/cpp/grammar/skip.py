import cpp

class SkipItem(cpp.yacc.Nonterm):
	"%nonterm"

	def skip_keyword(self, kw):
		"%reduce Keyword"
		self.value = kw.value
	def skip_operator(self, op):
		"%reduce Operator"
		self.value = op.value
	def skip_string(self, constant):
		"%reduce STRING"
		self.value = constant.value
	def skip_wstring(self, constant):
		"%reduce WSTRING"
		self.value = constant.value
	def skip_charconst(self, constant):
		"%reduce CHARCONST"
		self.value = constant.value
	def skip_wchar(self, constant):
		"%reduce WCHAR"
		self.value = constant.value
	def skip_decimal(self, constant):
		"%reduce DECIMAL"
		self.value = constant.value
	def skip_octal(self, constant):
		"%reduce OCTAL"
		self.value = constant.value
	def skip_hex(self, constant):
		"%reduce HEX"
		self.value = constant.value
	def skip_floating(self, constant):
		"%reduce FLOATING"
		self.value = constant.value
	def skip_id(self, value):
		"%reduce ID"
		self.value = value.value
	def skip_scope(self, value):
		"%reduce SCOPE"
		self.value = value.value
	def skiplist_semi(self, semi):
		"%reduce SEMI"
		self.value = semi.value

class SkipAllItem(cpp.yacc.Nonterm):
	"%nonterm"

	def skip_item(self, si):
		"%reduce SkipItem"
		self.value = si.value
	def skip_lt(self, op):
		"%reduce LT"
		self.value = op.value
	def skip_gt(self, op):
		"%reduce GT"
		self.value = op.value






class SkipListBase(cpp.yacc.Nonterm):
	"%nonterm"

	def skiplist_empty(self):
		"%reduce"
		self.value = ""
	def skiplist_list(self, skiplist, item):
		"%reduce SkipListBase SkipItem"
		self.value = skiplist.value + " " + item.value

class SkipListAllBase(cpp.yacc.Nonterm):
	"%nonterm"

	def skiplist_empty(self):
		"%reduce"
		self.value = ""
	def skiplist_list(self, skiplist, item):
		"%reduce SkipListAllBase SkipAllItem"
		self.value = skiplist.value + " " + item.value




class SkipList(cpp.yacc.Nonterm):
	"%nonterm"

	def skiplist_base(self, skiplist):
		"%reduce SkipListAllBase"
		self.value = skiplist.value

	def skiplist_paren(self, skiplist1, lparen, skiplist2, rparen, skiplist3):
		"%reduce SkipList LPAREN SkipList RPAREN SkipListAllBase"
		self.value = skiplist1.value + " ( " + skiplist2.value + " ) " + skiplist3.value

	def skiplist_brace(self, skiplist1, lbrace, skiplist2, rbrace, skiplist3):
		"%reduce SkipList LBRACE SkipList RBRACE SkipListAllBase"
		self.value = skiplist1.value + " { " + skiplist2.value + " } " + skiplist3.value

	def skiplist_brackets(self, skiplist1, lbracket, skiplist2, rbracket, skiplist3):
		"%reduce SkipList LBRACKET SkipList RBRACKET SkipListAllBase"
		self.value = skiplist1.value + " [ " + skiplist2.value + " ] " + skiplist3.value



class SkipListTemplate(cpp.yacc.Nonterm):
	"%nonterm"

	def skiplist_base(self, skiplist):
		"%reduce SkipListBase"
		self.value = skiplist.value

	def skiplist_paren(self, skiplist1, lparen, skiplist2, rparen, skiplist3):
		"%reduce SkipListTemplate LPAREN SkipList RPAREN SkipListBase"
		self.value = skiplist1.value + " ( " + skiplist2.value + " ) " + skiplist3.value

	def skiplist_brace(self, skiplist1, lbrace, skiplist2, rbrace, skiplist3):
		"%reduce SkipListTemplate LBRACE SkipList RBRACE SkipListBase"
		self.value = skiplist1.value + " { " + skiplist2.value + " } " + skiplist3.value

	def skiplist_brackets(self, skiplist1, lbracket, skiplist2, rbracket, skiplist3):
		"%reduce SkipListTemplate LBRACKET SkipList RBRACKET SkipListBase"
		self.value = skiplist1.value + " [ " + skiplist2.value + " ] " + skiplist3.value

	def skiplist_template(self, skiplist1, lt, skiplist2, gt, skiplist3):
		"%reduce SkipListTemplate LT SkipListTemplate GT SkipListBase"
		self.value = skiplist1.value + " < " + skiplist2.value + " > " + skiplist3.value
