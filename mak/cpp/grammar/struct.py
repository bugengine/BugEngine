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
        self.name = name.value.replace(' ', '')
        self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
        self.lineno = cls.lineno
        self.value = False
        self.pod = False
        self.members = members.members[4]
        if parent.inherits[0] >= 3:
            self.inherits = parent.inherits[1]
        else:
            self.inherits = "void"

    def struct_definition(self, cls, name, parent, lbrace, members, rbrace):
        "%reduce STRUCT NameOpt Parent LBRACE Members RBRACE"
        self.name = name.value.replace(' ', '')
        self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
        self.lineno = cls.lineno
        self.value = True
        self.pod = False
        self.members = members.members[3]
        if self.members and members.members[4]:
            self.members.members += members.members[4].members
            self.members.objects += members.members[4].objects
            for m, methods in members.members[4].methods.items():
                try:
                    self.members.methods[m] += methods
                except KeyError:
                    self.members.methods[m] = methods
        elif not self.members:
            self.members = members.members[4]
        if parent.inherits[0] >= 2:
            self.inherits = parent.inherits[1]
        else:
            self.inherits = "void"


    def pod_definition(self, cls, name, parent, lbrace, members, rbrace):
        "%reduce BE_POD NameOpt Parent LBRACE Members RBRACE"
        self.name = name.value.replace(' ', '')
        self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
        self.lineno = cls.lineno
        self.value = True
        self.pod = True
        self.members = members.members[3]
        if self.members and members.members[4]:
            self.members.members += members.members[4].members
            self.members.objects += members.members[4].objects
            for m, methods in members.members[4].methods.items():
                try:
                    self.members.methods[m] += methods
                except KeyError:
                    self.members.methods[m] = methods
        elif not self.members:
            self.members = members.members[4]
        if parent.inherits[0] >= 2:
            self.inherits = parent.inherits[1]
        else:
            self.inherits = "void"

    def union_definition(self, union, name, lbrace, members, rbrace):
        "%reduce UNION NameOpt LBRACE Members RBRACE"
        self.name = name.value.replace(' ', '')
        self.decl = name.value.replace(' ', '').replace(':', '_').replace('<', '__').replace('>', '__')
        self.lineno = union.lineno
        self.inherits = 'void'
        self.value = True
        self.pod = True
        self.members = members.members[3]
        if self.members and members.members[4]:
            self.members.members += members.members[4].members
            self.members.objects += members.members[4].objects
            for m, methods in members.members[4].methods.items():
                try:
                    self.members.methods[m] += methods
                except KeyError:
                    self.members.methods[m] = methods
        elif not self.members:
            self.members = members.members[4]

    def using(self, files, namespace, parent):
        parent = parent + [self.name]
        if len(parent) > 1:
            files[0].write('	typedef %s %s;\n' % ('::'.join(parent), self.name))
        if self.members:
            self.members.using(files, namespace, parent)

    def predecl(self, files, namespace, parent):
        parent = parent + [self.name]
        files[1].write('raw< ::BugEngine::RTTI::Class > %s_preklass();\n' % '_'.join(parent))
        files[1].write('raw< const ::BugEngine::RTTI::Class > %s_properties();\n' % '_'.join(parent))
        if self.members:
            self.members.predecl(files, namespace, parent, self.value, self)

    def create_pod_constructor(self, files, namespace, parent):
        decl = '_'.join(parent)
        method_name = '%s_constructor_overload_default' % (decl)
        files[0].write('static const ::BugEngine::RTTI::Method::Overload %s =\n' % method_name)
        files[0].write('{\n')
        files[0].write('	{0},\n')
        files[0].write('	{0},\n')
        files[0].write('	::BugEngine::be_typeid< %s >::type(),\n' % '::'.join(parent))
        files[0].write('	0,\n')
        files[0].write('	{0},\n')
        files[0].write('	0,\n')
        files[0].write('	&::BugEngine::RTTI::createPod< %s >\n' % '::'.join(parent))
        files[0].write('};\n')
        files[0].write('static const ::BugEngine::RTTI::Method::Parameter %s_constructor_overload_param =\n' % (decl))
        files[0].write('{\n')
        files[0].write('	{0},\n')
        files[0].write('	{0},\n')
        files[0].write('	\"other\",\n')
        files[0].write('	::BugEngine::be_typeid< const %s& >::type()\n' % '::'.join(parent))
        files[0].write('};\n')
        method_name = '%s_constructor_overload_copy' % ('_'.join(parent))
        files[0].write('static const ::BugEngine::RTTI::Method::Overload %s =\n' % method_name)
        files[0].write('{\n')
        files[0].write('	{0},\n')
        files[0].write('	{&%s_constructor_overload_default},\n' % decl)
        files[0].write('	::BugEngine::be_typeid< %s >::type(),\n' % '::'.join(parent))
        files[0].write('	1,\n')
        files[0].write('	{&%s_constructor_overload_param},\n' % decl)
        files[0].write('	0,\n')
        files[0].write('	&::BugEngine::RTTI::createPodCopy< %s >\n' % '::'.join(parent))
        files[0].write('};\n')
        files[0].write('static const ::BugEngine::RTTI::Method %s_constructor =\n' % decl)
        files[0].write('{\n')
        files[0].write('	::BugEngine::istring("?new"),\n')
        files[0].write('	{0},\n')
        files[0].write('	{&%s_constructor},\n' % decl)
        files[0].write('	%s,\n' % 2)
        files[0].write('	{&%s}\n' % method_name)
        files[0].write('};\n')
        return '{&%s_constructor}' % decl


    def dump(self, files, namespace, parent):
        if parent:
            owner = '::BugEngine::be_typeid< %s >::preklass()' % ('::'.join(namespace + parent))
        elif namespace:
            owner = '::BugEngine::be_%s_Namespace_%s()' % (self.parser.plugin, '_'.join(namespace))
        else:
            owner = '::BugEngine::be_%s_Namespace()' % self.parser.plugin
        parent = parent + [self.name]

        if self.members:
            self.members.dumpObjects(files, namespace, parent)
        else:
            pass

        files[0].write('raw< ::BugEngine::RTTI::Class > %s_preklass()\n' % '_'.join(parent))
        files[0].write('{\n')
        files[0].write('	static ::BugEngine::RTTI::Class klass = {\n')
        files[0].write('		::BugEngine::istring("%s"),\n' % self.name)
        files[0].write('		{%s.m_ptr},\n' % owner)
        files[0].write('		{::BugEngine::be_typeid< %s >::preklass().m_ptr},\n' % self.inherits)
        files[0].write('		u32(sizeof(%s)),\n' % '::'.join(parent))
        files[0].write('		i32(ptrdiff_t(static_cast<%s*>((%s*)(4))))-4,\n' % ('::'.join(parent), self.inherits))
        if self.tags.index:
            files[0].write('		%s,\n' % self.tags.index)
        elif self.pod:
            files[0].write('		::BugEngine::RTTI::ClassType_Pod,\n')
        elif self.value:
            files[0].write('		::BugEngine::RTTI::ClassType_Struct,\n')
        else:
            files[0].write('		::BugEngine::RTTI::ClassType_Object,\n')
        files[0].write('		{0},\n')
        files[0].write('		{0},\n')
        files[0].write('		{0},\n')
        files[0].write('		{0},\n')
        files[0].write('		{0},\n')
        files[0].write('		{0},\n')
        if self.value:
            files[0].write('		&::BugEngine::RTTI::wrapCopy< %s >,\n' % '::'.join(parent))
            files[0].write('		&::BugEngine::RTTI::wrapDestroy< %s >\n' % '::'.join(parent))
        else:
            files[0].write('		0,\n')
            files[0].write('		0\n')
        files[0].write('	};\n')
        files[0].write('	raw< ::BugEngine::RTTI::Class > result = { &klass };\n')
        files[0].write('	return result;\n')
        files[0].write('}\n')

        files[0].write('raw< const ::BugEngine::RTTI::Class > %s_properties()\n' % '_'.join(parent))
        files[0].write('{\n')
        i = 2
        while i <= len(parent):
            files[0].write('	typedef %s %s;\n' % ('::'.join(parent[:i]), parent[i-1]))
            i = i+1
        self.using(files, namespace, parent[:-1])
        files[0].write('	raw< ::BugEngine::RTTI::Class > result = %s_preklass();\n' % '_'.join(parent))
        if self.members:
            files[0].write('	raw< const ::BugEngine::RTTI::Class > parent = ::BugEngine::be_typeid< %s >::klass();\n' % self.inherits)
            objects, all_methods, properties = self.members.dump(files, namespace, parent, 'parent')
            methods, constructor, cast = all_methods
            if self.pod:
                constructor = self.create_pod_constructor(files, namespace, parent)
            if objects:
                files[0].write('	raw< const ::BugEngine::RTTI::ObjectInfo > objects = %s;\n' % objects)
                files[0].write('	result->objects.set(objects.operator->());\n')
            else:
                files[0].write('	result->objects = parent->objects;\n')
            if methods:
                files[0].write('	raw< const ::BugEngine::RTTI::Method > methods = %s;\n' % methods)
                files[0].write('	result->methods.set(methods.operator->());\n')
            else:
                files[0].write('	result->methods = parent->methods;\n')
            if constructor:
                files[0].write('	raw< const ::BugEngine::RTTI::Method > constructor = %s;\n' % constructor)
                files[0].write('	result->constructor.set(constructor.operator->());\n')
            if cast:
                files[0].write('	raw< const ::BugEngine::RTTI::Method > cast = %s;\n' % cast)
                files[0].write('	result->constructor.set(cast.operator->());\n')
            if properties:
                files[0].write('	raw< const ::BugEngine::RTTI::Property > properties = %s;\n' % properties)
                files[0].write('	result->properties.set(properties.operator->());\n')
            else:
                files[0].write('	result->properties = parent->properties;\n')
        if self.tags:
            tags = self.tags.dump(files, '%s_klass' % '_'.join(parent))
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
