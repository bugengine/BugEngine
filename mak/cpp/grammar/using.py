import cpp.yacc as yacc

class Using(yacc.Nonterm):
	"%nonterm"
	def using_name(self, using, name, semi):
		"%reduce USING Name SEMI"
	def using_namespace(self, using, namespace, name, semi):
		"%reduce USING NAMESPACE Name SEMI"

