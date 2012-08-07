import cpp

class Enum(cpp.yacc.Nonterm):
	"%nonterm"

	def enum_value(self, tags_left, id, equal, value, tags_right):
		"%reduce TagsLeft ID EQUAL Value TagsRight"
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags

	def enum(self, tags_left, id, tags_right):
		"%reduce TagsLeft ID TagsRight"
		self.name = id.value
		self.lineno = id.lineno
		self.tags = tags_left
		self.tags.tags += tags_right.tags



class EnumSequence(cpp.yacc.Nonterm):
	"%nonterm"

	def enumseq(self, enumseq, comma, tags, enum):
		"%reduce EnumSequence COMMA TagsRight Enum"
		self.enums = enumseq.enums
		self.enums.append(enum)
		enum.tags.tags += tags.tags

	def enumseq_1(self, enum):
		"%reduce Enum"
		self.enums = [enum]



class EnumValueList(cpp.yacc.Nonterm):
	"%nonterm"

	def empty(self):
		"%reduce"
		self.enums = []

	def enum_list(self, enums_sequence):
		"%reduce EnumSequence"
		self.enums = enums_sequence.enums

	def dump(self, file, instances, decl, owner, type):
		enums_pointer = "{0}"
		for enum in self.enums[::-1]:
			enum_tag = enum.tags.dump(file, instances, decl+"_enum_%s"%enum.name)
			file.write("#line %d\n"%enum.lineno)
			file.write("static const %s::%s s_%s_%s_value(%s::%s);\\\n" % (owner, type, decl, enum.name, owner, enum.name))
			alias_index = 0
			for name in [enum.name]+enum.tags.aliases:
				file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_enum_%s_%d = { %s, %s, \"%s\", ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(s_%s_%s_value)) };\n" % (decl, enum.name, alias_index, enums_pointer, enum_tag, name, decl, enum.name))
				enums_pointer = "{&s_%s_enum_%s_%d}" % (decl, enum.name, alias_index)
				alias_index += 1
		return enums_pointer




class EnumDef(cpp.yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, enums, rbrace):
		"%reduce ENUM NameOpt LBRACE EnumValueList RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = self.name
		self.value = enums
		self.lineno = enum.lineno

	def using(self, file, instances, decl, name, parent_name):
		pass

	def predecl(self, file, instances, decl, name, member):
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = '::'+'::'.join(name)
		decl = "enum%s" % '_'.join(decl)
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))

	def dump(self, file, instances, namespace, owner, decl, name, member):
		ns = '::'+'::'.join(namespace)
		owner_ = '::'+'::'.join(name)
		name = name+[self.name]
		decl = decl+[self.decl]
		fullname = '::'+'::'.join(name)
		prettyname = '.'.join(name)
		prefix = "enum%s" % '_'.join(decl)

		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, prefix)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % prefix)
		else:
			varname = "%s::s_%s" % (ns, prefix)
		if member:
			file.write("#line %d\n"%self.lineno)
			file.write("typedef %s %s;\n" % ('::'.join(name), self.name))

		tag_ptr = self.tags.dump(file, instances, prefix)
		properties = "{0}"
		objects = self.value.dump(file, instances, prefix, owner_, self.name)
		methods = "{0}"

		file.write("#line %d\n"%self.lineno)
		if self.parser.useMethods:
			file.write("static ")
		file.write("::BugEngine::RTTI::Class s_%s =\n" % (prefix))
		file.write("#line %d\n"%self.lineno)
		file.write("    {\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::istring(\"%s\"),\n" % (self.name))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % owner)
		file.write("#line %d\n"%self.lineno)
		file.write("        ::BugEngine::be_typeid< void >::klass(),\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        (u32)(sizeof(%s)),\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        0,\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (tag_ptr))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (properties))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (methods))
		file.write("#line %d\n"%self.lineno)
		file.write("        %s,\n" % (objects))
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        {0},\n")
		file.write("#line %d\n"%self.lineno)
		file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % fullname)
		file.write("#line %d\n"%self.lineno)
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % prefix)

		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return varname, tag_ptr
