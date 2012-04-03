import sys
try:
	import cpp.lexer as lexer
	import ply.yacc as yacc
except:
	import lexer
	import yacc



class Header(yacc.Nonterm):
	"%start"



spec = yacc.Spec(sys.modules[__name__],
				pickleFile="cpp.pickle",
				skinny=False,
				logFile="cpp.log",
				graphFile="cpp.dot",
				verbose=True)

class Parser(yacc.Lr):
	def __init__(self):
		yacc.Lr.__init__(self, spec)

	def parse(self, input, lexer):
		lexer.input(input)
		t=lexer.token()
		while t:
			tok = t.token(self)
			tok.value = t.value
			tok.lineno = t.lineno

			t=lexer.token()

