import cpp


class Tag(cpp.yacc.Nonterm):
	"%nonterm"

	def tag(self, tag, lparen, name, lparen2, skiplist, rparen2, rparen):
		"%reduce BE_TAG LPAREN Name LPAREN SkipList RPAREN RPAREN"
		self.value = (name.value, skiplist.value)


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

	def dump(self, file, instances, prefix):
		tag_ptr = "{0}"
		tag_index = 0
		for type,tag in self.tags:
			file.write("static %s s_%s_tag_value_%d = %s(%s);\n" % (type, prefix, tag_index, type, tag))
			file.write("static ::BugEngine::RTTI::Tag s_%s_tag_%d =\n" % (prefix, tag_index))
			file.write("    {\n")
			file.write("        %s,\n" % tag_ptr)
			file.write("        ::BugEngine::RTTI::Value(be_typeid< %s >::type(), (void*)&s_%s_tag_value_%d)\n" % (type, prefix, tag_index))
			file.write("    };\n")
			tag_ptr = "{&s_%s_tag_%d}" % (prefix, tag_index)
			tag_index += 1
		return tag_ptr

class TagsRight(cpp.yacc.Nonterm):
	"%nonterm"

	def taglistcomment(self, comment):
		"%reduce DoxyCommentRight"
		self.tags = []
