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
	def none(self):
		"%reduce"
		self.visibility = 2
	def public(self, public):
		"%reduce PUBLIC"
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
		self.members = members.members[4]
		if parent.inherits[0] >= 3:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def struct_definition(self, cls, name, parent, lbrace, members, rbrace):
		"%reduce STRUCT NameOpt Parent LBRACE Members RBRACE"
		self.name = name.value
		self.lineno = cls.lineno
		self.value = True
		self.members = members.members[3]
		if self.members and members.members[4]:
			self.members.members += members.members[4].members
			self.members.objects += members.members[4].objects
			for m, methods in members.members[4].methods.items():
				try:
					self.members.methods[m] += methods
				except KeyError:
					self.members.methods[m] = methods
		else:
			self.members = members.members[4]
		if parent.inherits[0] >= 2:
			self.inherits = parent.inherits[1]
		else:
			self.inherits = "void"

	def union_definition(self, union, name, lbrace, members, rbrace):
		"%reduce UNION NameOpt LBRACE Members RBRACE"
		self.name = name.value
		self.lineno = union.lineno
		self.inherits = 'void'
		self.value = True
		self.members = members.members

	def predecl(self, file, instances, name, member):
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		decl = "class%s" % fullname.replace(':', '_')
		if self.parser.useMethods:
			instances.write("extern const ::BugEngine::RTTI::Class& s_%sFun();\n" % decl)
		else:
			instances.write("extern ::BugEngine::RTTI::Class s_%s;\n" % (decl))
		if self.members:
			self.members.predecl(file, instances, name, self.value)


	def dump(self, file, instances, namespace, name, member, object_ptr):
		ns = '::'+'::'.join(namespace)
		name = name+[self.name]
		fullname = '::'+'::'.join(name)
		prettyname = '.'.join(name)
		decl = "class%s" % fullname.replace(':', '_')

		if member:
			file.write("#line %d\n"%self.lineno)
			file.write("typedef %s %s;\n" % ('::'.join(name), self.name))
		tag_ptr = self.tags.dump(file, instances, decl)
		if self.members:
			objects,methods,constructor,properties = self.members.dump(file, instances, namespace, name, fullname, self.value)
		else:
			objects = methods = constructor = properties = "{0}"

		if self.parser.useMethods:
			varname = "%s::s_%sFun()" % (ns, decl)
			file.write("const ::BugEngine::RTTI::Class& s_%sFun()\\\n{\\\n" % decl)
		else:
			varname = "%s::s_%s" % (ns, decl)

		file.write("#line %d\n"%self.lineno)
		file.write("::BugEngine::RTTI::Class s_%s =\\\n" % (decl))
		file.write("    {\\\n")
		file.write("        inamespace(\"%s\"),\\\n" % (prettyname))
		file.write("        be_typeid< %s >::klass(),\\\n" % (self.inherits))
		file.write("        be_checked_numcast<u32>(sizeof(%s)),\\\n" % fullname)
		file.write("        be_checked_numcast<i32>((ptrdiff_t)static_cast< %s* >((%s*)1)-1),\\\n" % (self.inherits, fullname))
		file.write("        %s,\\\n" % (tag_ptr))
		file.write("        %s,\\\n" % (properties))
		file.write("        %s,\\\n" % (methods))
		file.write("        %s,\\\n" % (objects))
		file.write("        %s,\\\n" % (constructor))
		if self.value:
			file.write("        &::BugEngine::RTTI::wrapCopy< %s >,\\\n" % fullname)
			file.write("        &::BugEngine::RTTI::wrapDestroy< %s >\\\n" % fullname)
		else:
			file.write("        0,\\\n")
			file.write("        0\\\n")
		file.write("    };\n")
		if self.parser.useMethods:
			file.write("return s_%s;\n}\n" % decl)
		alias_index = 0
		for name in self.aliases+[self.name]:
			alias_index += 1
			file.write("#line %d\n"%self.lineno)
			file.write("static ::BugEngine::RTTI::Class::ObjectInfo s_%s_obj_%d = { %s, %s, \"%s\", ::BugEngine::RTTI::Value(&%s) };\n" % (decl, alias_index, object_ptr, tag_ptr, name, varname))
			object_ptr = "{&s_%s_obj_%d}"%(decl,alias_index)

		instances.write("#line %d\n"%self.lineno)
		instances.write("template< > BE_EXPORT raw<const RTTI::Class> be_typeid< %s >::klass() { raw<const RTTI::Class> ci = {&%s}; return ci; }\n" % (fullname, varname))

		return object_ptr
