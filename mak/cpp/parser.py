import cpp.yacc
import rtti
import cpp.tokens
import cpp.grammar.unit
import cpp.grammar.namespace
import cpp.grammar.name
import cpp.grammar.using
import cpp.grammar.tag
import cpp.grammar.comment
import cpp.grammar.struct
import cpp.grammar.enum
import cpp.grammar.method
import cpp.grammar.variable
import cpp.grammar.type
import cpp.grammar.template
import cpp.grammar.keywords
import cpp.grammar.value
import cpp.grammar.skip


spec = cpp.yacc.Spec(
				[
					cpp.tokens,
					cpp.grammar.unit,
					cpp.grammar.namespace,
					cpp.grammar.name,
					cpp.grammar.using,
					cpp.grammar.tag,
					cpp.grammar.comment,
					cpp.grammar.struct,
					cpp.grammar.enum,
					cpp.grammar.method,
					cpp.grammar.variable,
					cpp.grammar.type,
					cpp.grammar.template,
					cpp.grammar.keywords,
					cpp.grammar.value,
					cpp.grammar.skip,
				],
				pickleFile="../../cpp.pickle",
				logFile="../../cpp.log",
				graphFile="../../cpp.dot",
				verbose=False)

class Parser(cpp.yacc.Lr):
	def __init__(self):
		cpp.yacc.Lr.__init__(self, spec)
		self.root = rtti.Root()

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

	def dump(self, implementation, instances, useMethods, name, source):
		self.root.dump(implementation, instances, useMethods, name, source)
		pass
