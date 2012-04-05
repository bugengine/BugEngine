import cpp.yacc
import cpp.tokens
import cpp.grammar.unit


spec = cpp.yacc.Spec([cpp.tokens, cpp.grammar.unit],
				pickleFile="cpp.pickle",
				logFile="cpp.log",
				graphFile="cpp.dot",
				verbose=True)

class Parser(cpp.yacc.Lr):
	def __init__(self):
		cpp.yacc.Lr.__init__(self, spec)

	def parse(self, input, lexer):
		lexer.input(input)
		t=lexer.token()
		while t:
			tok = cpp.tokens.__dict__[t.type](self)
			tok.value = t.value
			tok.lineno = t.lineno
			self.token(tok)
			t=lexer.token()
		self.eoi()
