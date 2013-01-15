import cpp


class Tag(cpp.yacc.Nonterm):
	"%nonterm"

	def tag(self, tag, lparen, name, lparen2, skiplist, rparen2, rparen):
		"%reduce BE_TAG LPAREN Name LPAREN SkipList RPAREN RPAREN"
		self.value = (name.value, skiplist.value, name.lineno)


class TagList(cpp.yacc.Nonterm):
	"%nonterm"

	def taglist_empty(self):
		"%reduce"
		self.tags = []
		self.aliases = []

	def taglist(self, taglist, tag):
		"%reduce TagList Tag"
		self.tags = taglist.tags
		self.aliases = taglist.aliases
		if tag.value[0] == 'Alias':
			if tag.value[1][0] == '"':
				self.aliases.append(tag.value[1][1:-1])
			else:
				self.aliases.append(tag.value[1])
		else:
			self.tags.append(tag.value)

class TagsLeft(cpp.yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment, taglist):
		"%reduce DoxyComment TagList"
		self.tags = taglist.tags
		self.aliases = taglist.aliases
		self.comment = comment

	def dump(self, files, namespace, owner):
		pass

class TagsRight(cpp.yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment):
		"%reduce DoxyCommentRight"
		self.tags = []
		self.comment = comment
