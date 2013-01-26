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

	def dump(self, files, namespace, parent):
		result = None
		for enum in self.enums:
			name = '%s_%s' % ('_'.join(parent), enum.name)
			tags = enum.tags.dump(files, name)
			for name in [enum.name] + enum.tags.aliases:
				new_name = '%s_%s' % ('_'.join(parent), name)
				files[0].write('static const ::BugEngine::RTTI::ObjectInfo %s =\n' % new_name)
				files[0].write('{\n')
				files[0].write('	%s,\n' % (result or '{0}'))
				files[0].write('	%s,\n' % tags)
				files[0].write('	::BugEngine::istring("%s"),\n' % name)
				if len(parent) > 1:
					files[0].write('	::BugEngine::RTTI::Value(%s::%s)\n' % ('::'.join(parent[:-1]), name))
				else:
					files[0].write('	::BugEngine::RTTI::Value(%s)\n' % (name))
				files[0].write('};\n')
				result = '{&%s}' % new_name

class EnumDef(cpp.yacc.Nonterm):
	"%nonterm"

	def enum(self, enum, name, lbrace, enums, rbrace):
		"%reduce ENUM NameOpt LBRACE EnumValueList RBRACE"
		self.name = name.value.replace(' ', '')
		self.decl = self.name
		self.value = enums
		self.lineno = enum.lineno

	def using(self, files, namespace, parent):
		parent = parent + [self.name]
		if len(parent) > 1:
			files[0].write('	typedef %s %s;\n' % ('::'.join(parent), self.name))

	def predecl(self, files, namespace, parent):
		parent = parent + [self.name]
		files[1].write('raw< ::BugEngine::RTTI::Class > %s_preklass();\n' % '_'.join(parent))
		files[1].write('raw< const ::BugEngine::RTTI::Class > %s_properties();\n' % '_'.join(parent))

	def dump(self, files, namespace, parent):
		if parent:
			owner = '::BugEngine::be_typeid< %s >::preklass()' % ('::'.join(namespace + parent))
		elif namespace:
			owner = '::BugEngine::be_%s_Namespace_%s()' % (self.parser.plugin, '_'.join(namespace))
		else:
			owner = '::BugEngine::be_%s_Namespace()' % self.parser.plugin
		parent = parent + [self.name]

		files[0].write('raw< ::BugEngine::RTTI::Class > %s_preklass()\n' % '_'.join(parent))
		files[0].write('{\n')
		files[0].write('	static ::BugEngine::RTTI::Class klass = {\n')
		files[0].write('		::BugEngine::istring("%s"),\n' % self.name)
		files[0].write('		{%s.m_ptr},\n' % owner)
		files[0].write('		::BugEngine::be_typeid< void >::preklass(),\n')
		files[0].write('		u32(sizeof(%s)),\n' % '::'.join(parent))
		files[0].write('		i32(0),\n')
		files[0].write('		{0},\n')
		files[0].write('		{0},\n')
		files[0].write('		{0},\n')
		files[0].write('		{0},\n')
		files[0].write('		{0},\n')
		files[0].write('		{0},\n')
		files[0].write('		&::BugEngine::RTTI::wrapCopy< %s >,\n' % '::'.join(parent))
		files[0].write('		&::BugEngine::RTTI::wrapDestroy< %s >\n' % '::'.join(parent))
		files[0].write('	};\n')
		files[0].write('	raw< ::BugEngine::RTTI::Class > result = { &klass };\n')
		files[0].write('	return result;\n')
		files[0].write('}\n')

		files[0].write('raw< const ::BugEngine::RTTI::Class > %s_properties()\n' % '_'.join(parent))
		files[0].write('{\n')
		files[0].write('	raw< ::BugEngine::RTTI::Class > result = %s_preklass();\n' % '_'.join(parent))
		objects = self.value.dump(files, namespace, parent)
		if objects:
			files[0].write('	raw< const ::BugEngine::RTTI::ObjectInfo > objects = %s;\n' % objects)
			files[0].write('	result->objects.set(objects.operator->());\n')
		if self.tags:
			tags = self.tags.dump(files, '%s_enum' % '_'.join(parent))
			files[0].write('	raw< ::BugEngine::RTTI::Tag > tags = %s;\n' % tags)
			files[0].write('	result->tags = tags;\n')
		files[0].write('	return result;\n')
		files[0].write('}\n')
		
		files[0].write('BE_EXPORT raw< const ::BugEngine::RTTI::Class > %s_create = ::BugEngine::be_typeid< %s >::klass();\n' % ('_'.join(parent), '::'.join(parent)))
		

		files[1].write('template<> BE_EXPORT raw<RTTI::Class> be_typeid< %s >::preklass()\n' % '::'.join(namespace + parent))
		files[1].write('{\n')
		files[1].write('	return %s::%s_preklass();\n' % ('::'.join(namespace), '_'.join(parent)))
		files[1].write('}\n')
		files[1].write('template<> BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::registerProperties()\n' % '::'.join(namespace + parent))
		files[1].write('{\n')
		files[1].write('	return %s::%s_properties();\n' % ('::'.join(namespace), '_'.join(parent)))
		files[1].write('}\n')

		return '%s_preklass()' % '_'.join(parent), '{0}'