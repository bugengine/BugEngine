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
		self.index = 0

	def taglist(self, taglist, tag):
		"%reduce TagList Tag"
		self.tags = taglist.tags
		self.aliases = taglist.aliases
		self.index = 0
		if tag.value[0] == 'Alias':
			if tag.value[1][0] == '"':
				self.aliases.append(tag.value[1][1:-1])
			else:
				self.aliases.append(tag.value[1])
		elif tag.value[0] == 'Index':
			self.index = tag.value[1]
		else:
			self.tags.append(tag.value)

class TagsLeft(cpp.yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment, taglist):
		"%reduce DoxyComment TagList"
		self.tags = taglist.tags
		self.index = taglist.index
		self.aliases = taglist.aliases
		self.comment = comment

	def dump(self, files, decl):
		tag_ptr = "{0}"
		tag_index = 0
		for type,tag,lineno in self.tags:
			files[0].write("static %s %s_tag_value_%d = %s(%s);\n" % (type, decl, tag_index, type, tag))
			files[0].write("static ::BugEngine::RTTI::Tag %s_tag_%d =\n" % (decl, tag_index))
			files[0].write("    {\n")
			files[0].write("        %s,\n" % tag_ptr)
			files[0].write("        ::BugEngine::RTTI::Value(be_typeid< %s >::type(), (void*)&%s_tag_value_%d)\n" % (type, decl, tag_index))
			files[0].write("    };\n")
			tag_ptr = "{&%s_tag_%d}" % (decl, tag_index)
			tag_index += 1
		return tag_ptr

class TagsRight(cpp.yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment):
		"%reduce DoxyCommentRight"
		self.tags = []
		self.comment = comment
