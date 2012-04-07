import cpp.yacc as yacc


class Tag(yacc.Nonterm):
	"%nonterm"

	def tag(self, tag, lparen, name, lparen2, skiplist, rparen2, rparen):
		"%reduce BE_TAG LPAREN Name LPAREN SkipList RPAREN RPAREN"
		tag = (name.value, skiplist.value)


class TagList(yacc.Nonterm):
	"%nonterm"

	def taglist_empty(self):
		"%reduce"
		self.tags = []

	def taglist(self, taglist, tag):
		"%reduce TagList Tag"
		self.tags = taglist.tags
		self.tags.append(tag.tag)

class TagsLeft(yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment, taglist):
		"%reduce DoxyComment TagList"
		self.tags = taglist.tags

class TagsRight(yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment):
		"%reduce DoxyCommentRight"
		self.tags = []