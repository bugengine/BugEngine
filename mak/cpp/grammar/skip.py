import cpp

class SkipItem(cpp.yacc.Nonterm):
    "%nonterm"

    def skip_keyword(self, kw):
        "%reduce Keyword"
        self.value = kw.value
    def skip_operator(self, op):
        "%reduce Operator"
        self.value = op.value
    def skip_constant(self, constant):
        "%reduce Constant"
        self.value = constant.value
    def skip_id(self, value):
        "%reduce ID"
        self.value = value.value
    def skip_scope(self, value):
        "%reduce SCOPE"
        self.value = value.value

class SkipMoreItem(cpp.yacc.Nonterm):
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

class SkipAllItem(cpp.yacc.Nonterm):
    "%nonterm"
    def skip_item(self, si):
        "%reduce SkipMoreItem"
        self.value = si.value
    def skip_semi(self, kw):
        "%reduce SEMI"
        self.value = kw.value
    def skip_comma(self, kw):
        "%reduce COMMA"
        self.value = kw.value
    def skip_doxy_begin(self, kw):
        "%reduce DOXY_BEGIN"
        self.value = kw.value
    def kw_doxy_begin_left(self, kw):
        "%reduce DOXY_BEGIN_LEFT"
        self.value = kw.value
    def skip_doxy_end(self, kw):
        "%reduce DOXY_END"
        self.value = kw.value
    def skip_doxy_word(self, kw):
        "%reduce DOXY_WORD"
        self.value = kw.value
    def skip_doxy_newline(self, kw):
        "%reduce DOXY_NEWLINE"
        self.value = kw.value
    def skip_doxy_list(self, kw):
        "%reduce DOXY_LIST"
        self.value = kw.value





class SkipListTemplateBase(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist_empty(self):
        "%reduce"
        self.value = ""
    def skiplist_list(self, skiplist, item):
        "%reduce SkipListTemplateBase SkipItem"
        self.value = skiplist.value + " " + item.value
    def skiplist_comma(self, skiplist, comma):
        "%reduce SkipListTemplateBase COMMA"
        self.value = skiplist.value + " " + comma.value


class SkipListMoreBase(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist_empty(self):
        "%reduce"
        self.value = ""
    def skiplist_list(self, skiplist, item):
        "%reduce SkipListMoreBase SkipMoreItem"
        self.value = skiplist.value + " " + item.value

class SkipListBase(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist(self):
        "%reduce"
        self.value = ""

    def skiplist_comma(self, skiplistbase, item):
        "%reduce SkipListBase SkipAllItem"
        self.value = skiplistbase.value + item.value



class SkipListNoComma(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist_base(self, skiplist):
        "%reduce SkipListMoreBase"
        self.value = skiplist.value

    def skiplist_paren(self, skiplist1, lparen, skiplist2, rparen, skiplist3):
        "%reduce SkipListNoComma LPAREN SkipList RPAREN SkipListMoreBase"
        self.value = skiplist1.value + " ( " + skiplist2.value + " ) " + skiplist3.value

    def skiplist_brace(self, skiplist1, lbrace, skiplist2, rbrace, skiplist3):
        "%reduce SkipListNoComma LBRACE SkipList RBRACE SkipListMoreBase"
        self.value = skiplist1.value + " { " + skiplist2.value + " } " + skiplist3.value

    def skiplist_brackets(self, skiplist1, lbracket, skiplist2, rbracket, skiplist3):
        "%reduce SkipListNoComma LBRACKET SkipList RBRACKET SkipListMoreBase"
        self.value = skiplist1.value + " [ " + skiplist2.value + " ] " + skiplist3.value



class SkipListTemplate(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist_base(self, skiplist):
        "%reduce SkipListTemplateBase"
        self.value = skiplist.value

    def skiplist_paren(self, skiplist1, lparen, skiplist2, rparen, skiplist3):
        "%reduce SkipListTemplate LPAREN SkipList RPAREN SkipListTemplateBase"
        self.value = skiplist1.value + " ( " + skiplist2.value + " ) " + skiplist3.value

    def skiplist_brackets(self, skiplist1, lbracket, skiplist2, rbracket, skiplist3):
        "%reduce SkipListTemplate LBRACKET SkipList RBRACKET SkipListTemplateBase"
        self.value = skiplist1.value + " [ " + skiplist2.value + " ] " + skiplist3.value

    def skiplist_template(self, skiplist1, lt, skiplist2, gt, skiplist3):
        "%reduce SkipListTemplate LT SkipListTemplate GT SkipListTemplateBase"
        self.value = skiplist1.value + " < " + skiplist2.value + " > " + skiplist3.value

class SkipList(cpp.yacc.Nonterm):
    "%nonterm"

    def skiplist(self, skiplistbase):
        "%reduce SkipListBase"
        self.value = skiplistbase.value

    def skiplist_paren(self, skiplist1, lparen, skiplist2, rparen, skiplist3):
        "%reduce SkipList LPAREN SkipList RPAREN SkipListBase"
        self.value = skiplist1.value + " ( " + skiplist2.value + " ) " + skiplist3.value

    def skiplist_brace(self, skiplist1, lbrace, skiplist2, rbrace, skiplist3):
        "%reduce SkipList LBRACE SkipList RBRACE SkipListBase"
        self.value = skiplist1.value + " { " + skiplist2.value + " } " + skiplist3.value

    def skiplist_brackets(self, skiplist1, lbracket, skiplist2, rbracket, skiplist3):
        "%reduce SkipList LBRACKET SkipList RBRACKET SkipListBase"
        self.value = skiplist1.value + " [ " + skiplist2.value + " ] " + skiplist3.value


