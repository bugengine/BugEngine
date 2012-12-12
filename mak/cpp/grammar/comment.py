import cpp

class DoxyWords(cpp.yacc.Nonterm):
    "%nonterm"

    def words(self):
        "%reduce"

    def wordlist(self, words, word):
        "%reduce DOXY_WORD DoxyWords"

    def listitem(self, words, word):
        "%reduce DOXY_LIST DoxyWords"

    def newline(self, words, word):
        "%reduce DOXY_NEWLINE DoxyWords"




class DoxyCommentRight(cpp.yacc.Nonterm):
    "%nonterm"

    def doxy(self):
        "%reduce"
        self.comment = ""

    def doxyone(self, begin, words, end):
        "%reduce DOXY_BEGIN_LEFT DoxyWords DOXY_END"
        self.comment = ""

    def doxylong(self, begin, words, end, begin2, words2, end2):
        "%reduce DOXY_BEGIN_LEFT DoxyWords DOXY_END DOXY_BEGIN_LEFT DoxyWords DOXY_END"
        self.comment = ""



class DoxyComment(cpp.yacc.Nonterm):
    "%nonterm"

    def doxy(self):
        "%reduce"
        self.comment = ""

    def doxyone(self, begin, words, end):
        "%reduce DOXY_BEGIN DoxyWords DOXY_END"
        self.comment = ""

    def doxylong(self, begin, words, end, begin2, words2, end2):
        "%reduce DOXY_BEGIN DoxyWords DOXY_END DOXY_BEGIN DoxyWords DOXY_END"
        self.comment = ""

