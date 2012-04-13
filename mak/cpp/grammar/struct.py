import cpp


class MemberList(cpp.yacc.Nonterm):
	"%nonterm"

	def empty(self):
		"%reduce"
		self.members = [None, None, None, None, None]

	def block(self, visibility, colon, exprs, memberlist):
		"%reduce Visibility COLON Exprs MemberList"
		self.members = memberlist.members
		if self.members[visibility.visibility]:
			self.members[visibility.visibility].members += exprs.members
			self.members[visibility.visibility].objects += exprs.objects
			for m, methods in exprs.methods.items():
				try:
					self.members[visibility.visibility].methods[m] += methods
				except KeyError:
					self.members[visibility.visibility].methods[m] = methods
		else:
			self.members[visibility.visibility] = exprs


class Members(cpp.yacc.Nonterm):
	"%nonterm"

	def list(self, exprs, members):
		"%reduce Exprs MemberList"
		self.members = members.members
		self.members[3] = exprs

class Visibility(cpp.yacc.Nonterm):
	"%nonterm"

	def private(self, private):
		"%reduce PRIVATE"
		self.visibility = 0
	def protected(self, protected):
		"%reduce PROTECTED"
		self.visibility = 1
	def public(self, public):
		"%reduce PUBLIC"
		self.visibility = 2
	def none(self):
		"%reduce"
		self.visibility = 3
	def published(self, published):
		"%reduce PUBLISHED"
		self.visibility = 4




class ParentList(cpp.yacc.Nonterm):
	"%nonterm"

	def parent_item(self, visibility, name):
		"%reduce Visibility Name"
		self.inherits = (visibility.visibility, name.value)
	def parent_list(self, parent_list, comma, visibility, name):
		"%reduce ParentList COMMA Visibility Name"
		if visibility.visibility > parent_list.inherits[0]:
			self.inherits = (visibility.visibility, name.value)
		else:
			self.inherits = parent_list.inherits




class Parent(cpp.yacc.Nonterm):
	"%nonterm"

	def parent_none(self):
		"%reduce"
		self.inherits = (0, "")
	def parent_list(self, colon, parent_list):
		"%reduce COLON ParentList"
		self.inherits = parent_list.inherits




class ClassDef(cpp.yacc.Nonterm):
	"%nonterm"

	def class_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce CLASS NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value
		self.lineno = cls.lineno
		self.value = False
		if parent.inherits[0] >= 4:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def struct_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce STRUCT NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value
		self.lineno = cls.lineno
		self.value = True
		if parent.inherits[0] >= 3:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def union_definition(self, union, name, lbrace, members, rbrace):
		"%reduce UNION NameOpt LBRACE Members RBRACE"
		self.name = name.value
		self.lineno = union.lineno
		self.inherits = 'void'
		self.value = True

	def predecl(self, file, instances, name, owner, member):
		#TODO: using sub objects
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		decl = "class%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))


	def dump(self, file, instances, name, owner, member):
		namespace = '::'+'::'.join(name)
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		prettyname = '.'.join(name)

		tagname = "0"
		properties = "0"
		objects = "0"
		methods = constructor = destructor = call = "0"

		decl = "class%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (namespace, decl)
			file.write("static const ::BugEngine::RTTI::Class& s_%sFun()\n{\n" % decl)
		else:
			varname = "%s::s_%s" % (namespace, decl)

		file.write("::BugEngine::RTTI::Class s_%s =\n" % (decl))
		file.write("    {\n")
		file.write("        inamespace(\"%s\"),\n" % (prettyname))
		file.write("        be_typeid< %s >::klass(),\n" % (self.inherits))
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\n" % fullname)
		file.write("        be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1),\n" % (self.inherits, fullname))
		file.write("        {%s},\n" % (tagname))
		file.write("        {%s},\n" % (properties))
		file.write("        {%s},\n" % (methods))
		file.write("        {%s},\n" % (objects))
		file.write("        {%s},\n" % (constructor))
		file.write("        {%s},\n" % (destructor))
		if self.value:
			file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\n" % fullname)
			file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\n" % fullname)
		else:
			file.write("        0,\n")
			file.write("        0\n")
		file.write("    };\n")
		file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj = { %s->objects, {%s}, \"%s\", ::BugEngine::RTTI::Value(&s_%s) };\n" % (decl, owner, tagname, self.name, decl))
		file.write("const ::BugEngine::RTTI::Class::ObjectInfo* s_%s_obj_ptr = ( %s->objects.set(&s_%s_obj) );\n" % (decl, owner, decl))
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % decl)

		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))
