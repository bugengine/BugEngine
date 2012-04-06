import cpp.yacc as yacc
import rtti

class Namespace(yacc.Nonterm):
	"%nonterm"
	def namespace_named(self, namespace, name, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE ID LBRACE Exprs RBRACE"
		self.value = rtti.Namespace(name.value, name.lineno)
		for obj in exprs.values:
			self.value.addObject(obj)
	def namespace_unnamed(self, namespace, LBRACE, exprs, RBRACE):
		"%reduce NAMESPACE LBRACE RBRACE"
		#unnamed namespaces are not exxported
		self.value = None


