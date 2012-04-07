import cpp.yacc as yacc


class Template(yacc.Nonterm):
	"%nonterm"

	def template(self, template, lt, SkipListTemplate, gt):
		"%reduce TEMPLATE LT SkipListTemplate GT"
