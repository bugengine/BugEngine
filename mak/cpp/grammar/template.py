import cpp


class Template(cpp.yacc.Nonterm):
	"%nonterm"

	def template(self, template, lt, SkipListTemplate, gt):
		"%reduce TEMPLATE LT SkipListTemplate GT"
