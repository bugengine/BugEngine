def helper_name(struct_owners):
    if struct_owners:
        return '::'.join(i.helper_name() for i in struct_owners) + '::'
    else:
        return ''

def helper_static(struct_owners):
    if struct_owners:
        return ''
    else:
        return 'static '

class CppObject(object):
    def __init__(self):
        self.tags = []
        self.aliases = []
        self.index = None

    def add_tag(self, tag):
        if tag[0] == 'Alias':
            self.aliases.append(tag[1])
        elif tag[0] == 'Index':
            self.index = tag[1]
        else:
            self.tags.append(tag)

    def add_tags(self, tags):
        for t in tags:
            self.add_tag(t)

    def all_names(self):
        result = [(self.engine_name(), self.id())]
        for alias in self.aliases:
            result.append(('::BugEngine::istring(%s)' % alias,
                           alias.strip().replace('#', '_').replace('?', '_')
                                        .replace('"', '_').replace(':', '_')))
        return result

    def short_name(self):
        return self.name

    def id(self):
        return self.short_name().replace('#', '_').replace('?', '_').replace('"', '_').replace(':', '_')

    def engine_name(self):
        return '::BugEngine::istring("%s")' % self.short_name()

    def do_typedef(self, name, definition):
        pass

    def declare_namespace(self, definition):
        pass

    def declare(self, owner, struct_owners, definition, instance, prefix):
        if self.tags and struct_owners:
            definition.write('static const ::BugEngine::RTTI::Tag s%s_tags[];\n' % prefix)
            definition.write('static const ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Tag > s%s_tags_array;\n' % prefix)

    def write_tags(self, definition, struct_owners, prefix):
        if self.tags:
            definition.write('%sconst ::BugEngine::RTTI::Tag %ss%s_tags[] = {\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix))
            definition.write(',\n'.join(['    { ::BugEngine::RTTI::Value(%s(%s)) }' % (t[0], t[1]) for t in self.tags]))
            definition.write('\n};\n')
            definition.write('%sconst ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Tag > %ss%s_tags_array =\n'
                             '{ %d, s%s_tags };\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix, len(self.tags), prefix))
            return '{&s%s_tags_array }' % (prefix)
        else:
            return '{ 0 }'

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owners, prefix,namespace, object_name, definition, instance):
        return object_name


class Parameter(CppObject):
    def __init__(self, type, name, default_value):
        super(Parameter, self).__init__()
        self.type = type
        self.name = name
        self.default_value = default_value


class Method(CppObject):
    def __init__(self, name, return_type, parameters, attributes):
        super(Method, self).__init__()
        self.name = name
        self.return_type = return_type.strip()
        self.parameters = parameters[:]
        self.attributes = attributes
        self.vararg = self.parameters and self.parameters[-1].type == '...'
        if self.vararg:
            self.parameters.pop(-1)

    def add_attributes(self, attributes):
        self.attributes += attributes

    def cpp_name(self, owner):
        if owner:
            return '%s::%s' % (owner.cpp_name(), self.name)
        else:
            return self.name

    def trampoline_name(self, owner, prefix):
        return 'trampoline%s' % prefix

    def extra_params(self, owner, struct_owners):
        return []

    def has_trampoline(self):
        return True

    def declare(self, owner, struct_owners, definition, instance, prefix):
        CppObject.declare(self, owner, struct_owners, definition, instance, prefix)
        if self.has_trampoline():
            definition.write('    static ::BugEngine::RTTI::Value %s'
                             '(::BugEngine::RTTI::Value* parameters, u32 parameterCount);\n' % self.trampoline_name(owner, prefix))
        if struct_owners:
            param_index = 0
            if not 'static' in self.attributes and not 'builtin' in self.attributes:
                param_index = param_index + 1
            for p in self.parameters:
                param_index = param_index + 1
                p.declare(owner, struct_owners, definition, instance, prefix + '_param%d' % param_index)
                if p.default_value:
                    definition.write('    static const ::BugEngine::RTTI::Value s%s_param%d_default;\n' % (prefix, param_index))
            if param_index:
                definition.write('    static const ::BugEngine::RTTI::Method::Parameter s%s_params[];\n' % prefix)

    def call(self, owner, struct_owners):
        if struct_owners and not 'static' in self.attributes and not 'builtin' in self.attributes:
            params = ', '.join(self.extra_params(owner, struct_owners) +
                               ['parameters[%d].as< %s >()' % (i+1, p.type)
                                for i, p in enumerate(self.parameters)])
            if 'const' in self.attributes:
                return 'parameters[0].as< const %s& >().%s(%s)' % (owner.cpp_name(),
                                                                   self.cpp_name(owner),
                                                                   params)
            else:
                return 'parameters[0].as< %s& >().%s(%s)' % (owner.cpp_name(),
                                                             self.cpp_name(owner),
                                                             params)
        else:
            params = ', '.join(self.extra_params(owner, struct_owners) +
                               ['parameters[%d].as< %s >()' % (i, p.type)
                                for i, p in enumerate(self.parameters)])
            return '%s(%s)' % (self.cpp_name(owner), params)

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        if self.has_trampoline():
            definition.write('::BugEngine::RTTI::Value %s%s'
                             '(::BugEngine::RTTI::Value* parameters, u32 parameterCount)\n'
                             '{\n'
                             '    be_forceuse(parameters);\n'
                             '    be_forceuse(parameterCount);\n'
                              % (helper_name(struct_owners), self.trampoline_name(owner, prefix)))
            if self.return_type != 'void':
                if self.return_type[-1] == '&':
                    definition.write('    return ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s));\n'
                                     '}\n' % self.call(owner, struct_owners))
                else:
                    definition.write('    return ::BugEngine::RTTI::Value(%s);\n'
                                     '}\n' % self.call(owner, struct_owners))
            else:
                definition.write('    %s;\n'
                                 '    return ::BugEngine::RTTI::Value();\n'
                                 '}\n' % self.call(owner, struct_owners))

    def write_parameters(self, owner, struct_owners, prefix, definition):
        params = []
        param_index = 0
        if struct_owners and not 'static' in self.attributes and not 'builtin' in self.attributes:
            param_index = param_index + 1
            if 'const' in self.attributes:
                params.append((Parameter('const %s&'%owner.cpp_name(), 'this', None), '{ 0 }'))
            else:
                params.append((Parameter('%s&'%owner.cpp_name(), 'this', None), '{ 0 }'))
        for p in self.parameters:
            param_index = param_index + 1
            params.append((p, p.write_tags(definition, struct_owners, prefix + '_param%d' % param_index)))

        if params:
            for i, p in enumerate(params):
                p, t = p
                if p.default_value:
                    definition.write('%sconst ::BugEngine::RTTI::Value %ss%s_param%d_default ((%s)%s);\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix, i+1, p.type, p.default_value))
            definition.write('%sconst ::BugEngine::RTTI::Method::Parameter %ss%s_params[] = {\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix))
            definition.write(',\n'.join(['    {\n'
                                         '        %s,\n'
                                         '        ::BugEngine::istring("%s"),\n'
                                         '        ::BugEngine::be_typeid< %s >::type(),\n'
                                         '        {%s}\n'
                                         '    }' % (t, p[0].name, p[0].type,
                                                            p[0].default_value and ('&s%s_param%d_default'%(prefix, i+1)) or '&::BugEngine::RTTI::Method::Parameter::s_noDefaultValue') for i, p in enumerate(params)]))
            definition.write('\n};\n')
            return '{ %d, s%s_params }' % (len(params), prefix)
        else:
            return '{ 0, 0 }'


class BuiltinMethod(Method):
    def __init__(self, name, return_type, parameters, attributes):
        super(BuiltinMethod, self).__init__(name, return_type, parameters, attributes + ['builtin'])

    def cpp_name(self, owner):
        return '%s' % (self.name)


class Operator(Method):
    operator_map = {
        '()'    :   'Call',
        '[]'    :   'Index',
        '<'     :   'LessThan',
        '>'     :   'GreaterThan',
        '<='    :   'LessThanOrEqual',
        '>='    :   'GreaterThanOrEqual',
        '*'     :   'Multiply',
        '/'     :   'Divide',
        '%'     :   'Modulo',
        '+'     :   'Add',
        '-'     :   'Substract',
        '<<'    :   'ShiftLeft',
        '>>'    :   'ShiftRight',
        '&'     :   'BitwiseAnd',
        '|'     :   'BitwiseOr',
        '^'     :   'BitwiseXor',
        '~'     :   'BitwiseNot',
        '&&'    :   'LogicalAnd',
        '||'    :   'LogicalOr',
        '!'     :   'LogicalNot',
        '=='    :   'Equal',
        '!='    :   'NotEqual',
        '='     :   'Assign',
        '*='    :   'MultiplyAssign',
        '/='    :   'DivideAssign',
        '%='    :   'ModuloAssign',
        '+='    :   'AddAssign',
        '-='    :   'SubstractAssign',
        '<<='   :   'ShiftLeftAssign',
        '>>='   :   'ShiftRightAssign',
        '&='    :   'AndAssign',
        '|='    :   'OrAssign',
        '^='    :   'XorAssign',
        '++'    :   'Increment',
        '--'    :   'Decrement',
        '->'    :   'Get',
    }
    def __init__(self, operator_name, return_type, parameters, attributes):
        super(Operator, self).__init__(self.operator_map[operator_name],
                                       return_type, parameters, attributes)
        self.operator_name = operator_name

    def cpp_name(self, owner):
        if owner:
            return '%s::operator%s' % (owner.cpp_name(), self.operator_name)
        else:
            return 'operator%s'%self.operator_name

    def engine_name(self):
        return '::BugEngine::RTTI::Class::nameOperator%s()' % self.name


class Constructor(Method):
    def __init__(self, owner, name, return_type, parameters, attributes):
        if owner.type == 'class':
            return_type = 'ref< %s >' % return_type
        super(Constructor, self).__init__(name.strip(), return_type, parameters,
                                          attributes + ['static'])

    def cpp_name(self, owner):
        if owner.type == 'class':
            return 'ref< %s >::create' % (owner.cpp_name())
        else:
            return self.name

    def engine_name(self):
        return '::BugEngine::RTTI::Class::nameConstructor()'

    def extra_params(self, owner, struct_owners):
        if owner.type == 'class':
            return ['::BugEngine::Arena::general()']
        else:
            return []


class PodConstructor(Constructor):
    def __init__(self, owner):
        super(PodConstructor, self).__init__(owner, owner.name[-1], owner.name[-1], [], [])

    def has_trampoline(self):
        return False

    def trampoline_name(self, owner, prefix):
        return '::BugEngine::RTTI::createPod< %s >' % owner.cpp_name()



class PodConstructorCopy(Constructor):
    def __init__(self, owner):
        super(PodConstructorCopy, self).__init__(owner, owner.name[-1], owner.name[-1],
                                                 [Parameter('const %s&' % owner.cpp_name(), 'other', None)], [])

    def has_trampoline(self):
        return False

    def trampoline_name(self, owner, prefix):
        return '::BugEngine::RTTI::createPodCopy< %s >' % owner.cpp_name()


class Destructor(Method):
    def __init__(self, name, return_type, parameters, attributes):
        super(Destructor, self).__init__('destroy_%s' % name.strip(), return_type, parameters,
                                         attributes)

    def cpp_name(self, owner):
        return '~%s' % (owner.name[-1])

    def engine_name(self):
        return '::BugEngine::RTTI::Class::nameDestructor()'


class OverloadedMethod(CppObject):
    def __init__(self, method, alias, alias_cpp):
        super(OverloadedMethod, self).__init__()
        self.name = alias
        self.name_cpp = alias_cpp
        self.overloads = [method]

    def short_name(self):
        return self.name_cpp

    def declare(self, owner, struct_owners, definition, instance, prefix, member=False):
        prefix = prefix + '_method_%s' % self.name_cpp
        CppObject.declare(self, owner, struct_owners, definition, instance, prefix)
        for overload_index, o in enumerate(self.overloads):
            o.declare(owner, struct_owners, definition, instance, prefix+ '_overload_%d' % (overload_index))
        if struct_owners:
            definition.write('    static const ::BugEngine::RTTI::Method::Overload s%s_overloads[];\n' % prefix)
            if not member:
                definition.write('    static const ::BugEngine::RTTI::Method s%s_method;\n' % prefix)
                definition.write('    static const raw<const ::BugEngine::RTTI::Method> s%s_ptr;\n' % prefix)
                definition.write('    static const ::BugEngine::RTTI::ObjectInfo s%s_object;\n' % prefix)

    def add_overload(self, overload):
        self.overloads.append(overload)

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        prefix = prefix + '_method_%s' % self.name_cpp
        for i, o in enumerate(self.overloads):
            o.write_content(owner, struct_owners, prefix+ '_overload_%d' % (i), namespace, definition, instance)

    def write_overloads(self, owner, struct_owners, prefix, definition):
        overloads = []
        overload_index = 0
        for o in self.overloads:
            overloads.append((o,
                              o.write_tags(definition, struct_owners, prefix + '_overload_%d' % (overload_index)),
                              o.write_parameters(owner, struct_owners, prefix + '_overload_%d' % (overload_index), definition)))
            overload_index += 1
        definition.write('%sconst ::BugEngine::RTTI::Method::Overload %ss%s_overloads[] = {\n'%(helper_static(struct_owners), helper_name(struct_owners), prefix))
        definition.write(',\n'.join(['    {\n'
                                     '        %s,\n'
                                     '        %s,\n'
                                     '        ::BugEngine::be_typeid< %s >::type(),\n'
                                     '        %s,\n'
                                     '        &%s\n'
                                     '    }' % (o[1], o[2], o[0].return_type, o[0].vararg and "true" or "false", o[0].trampoline_name(owner, prefix + '_overload_%d'%i)) for i, o, in enumerate(overloads)]))
        definition.write('\n};\n')
        return '{ %d, s%s_overloads }' % (len(overloads), prefix)

    def write_object(self, owner, struct_owners, prefix, namespace, object_name, definition, instance):
        prefix = prefix + '_method_%s' % self.name_cpp
        method_ptr = self.write_method(owner, struct_owners, prefix, definition)
        definition.write('const %sraw<const ::BugEngine::RTTI::Method> %ss%s_ptr = {&%s};\n'
                         '%sconst ::BugEngine::RTTI::ObjectInfo %ss%s_object = {\n'
                         '    %s,\n'
                         '    {0},\n'
                         '    s%s_method.name,\n'
                         '    ::BugEngine::RTTI::Value(s%s_ptr)\n'
                         '};\n' % (helper_static(struct_owners), helper_name(struct_owners),
                                   prefix, method_ptr,
                                   helper_static(struct_owners), helper_name(struct_owners),
                                   prefix, object_name, prefix, prefix))
        return '{&%ss%s_object}' % (helper_name(struct_owners), prefix)

    def write_method(self, owner, struct_owners, prefix, definition):
        overload_array = self.write_overloads(owner, struct_owners, prefix, definition)
        definition.write('%sconst ::BugEngine::RTTI::Method %ss%s_method  = {\n'
                         '   %s,\n'
                         '   %s,\n'
                         '   {&s%s_method}\n'
                         '};\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix,
                                   self.name, overload_array, prefix))
        return 's%s_method' % prefix


class Variable(CppObject):
    def __init__(self, name, type):
        super(Variable, self).__init__()
        self.name = name
        self.type = type
        self.attributes = []

    def declare(self, owner, struct_owners, definition, instance, prefix):
        prefix = prefix + '_prop_' + self.name
        CppObject.declare(self, owner, struct_owners, definition, instance, prefix)
        if struct_owners and 'static' in self.attributes:
            for alias, alias_cpp in self.all_names():
                definition.write('    static const ::BugEngine::RTTI::ObjectInfo s%s_object_%s;\n' % (prefix, alias_cpp))

    def add_attributes(self, attributes):
        self.attributes += attributes

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owners, prefix, namespace, object_name, definition, instance):
        prefix = prefix + '_prop_' + self.name
        tag = self.write_tags(definition, struct_owners, prefix)
        for alias, alias_cpp in self.all_names():
            definition.write('%sconst ::BugEngine::RTTI::ObjectInfo %ss%s_object_%s = {\n'
                             '    %s,\n'
                             '    %s,\n'
                             '    ::BugEngine::istring(%s),\n'
                             '    ::BugEngine::RTTI::Value(\n'
                             '        ::BugEngine::RTTI::Value::ByRef(%s::%s))\n'
                             '};\n' % (hlper_static(struct_owners), helper_name(struct_owners),
                                       prefix, alias_cpp, object_name, tag,
                                       alias, owner.cpp_name(), self.name))
            object_name = '{&%ss%s_object_%s}' % (helper_name(struct_owners), prefix, alias_cpp)
        return object_name


class Typedef(CppObject):
    def __init__(self, name, type):
        super(Typedef, self).__init__()
        self.name = name
        self.type = type
        self.attributes = ['static']

    def do_typedef(self, names, definition):
        names = names + [self.name]
        definition.write('    typedef %s %s;\n' % ('::'.join(names), self.name))

    def declare(self, owner, struct_owners, definition, instance, prefix):
        prefix = prefix + '_' + self.name;
        CppObject.declare(self, owner, struct_owners, definition, instance, prefix)
        if struct_owners:
            for alias, alias_cpp in self.all_names():
                definition.write('    static const ::BugEngine::RTTI::ObjectInfo s%s_object_%s;\n' % (prefix, alias_cpp))

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owners, prefix, namespace, object_name, definition, instance):
        prefix = prefix + '_' + self.name
        tag = self.write_tags(definition, struct_owners, prefix)
        for alias, alias_cpp in self.all_names():
            definition.write('%sconst ::BugEngine::RTTI::ObjectInfo %ss%s_object_%s = {\n'
                             '    %s,\n'
                             '    %s,\n'
                             '    ::BugEngine::istring(%s),\n'
                             '    ::BugEngine::RTTI::Value(\n'
                             '        ::BugEngine::be_typeid< %s >::type())\n'
                             '};\n' % (helper_static(struct_owners), helper_name(struct_owners),
                                       prefix, alias_cpp, object_name, tag, alias, self.name))
            object_name = '{&%ss%s_object_%s}' % (helper_name(struct_owners), prefix, alias_cpp)
        return object_name


class EnumValue(Variable):
    def __init__(self, name, value, type):
        super(EnumValue, self).__init__(name, type)
        self.value = value
        self.add_attributes(['static'])

    def declare(self, owner, struct_owners, definition, instance, prefix):
        prefix = prefix + '_' + self.name
        CppObject.declare(self, owner, struct_owners, definition, instance, prefix)
        for alias, alias_cpp in self.all_names():
            definition.write('    static const ::BugEngine::RTTI::ObjectInfo s%s_object_%s;\n' % (prefix, alias_cpp))

    def cpp_name(self, owner):
        return '%s::%s' % ('::'.join(owner.cpp_name().split('::')[:-1]), self.name)

    def write_object(self, owner, struct_owners, prefix, namespace, object_name, definition, instance):
        prefix = prefix + '_' + self.name
        n = self.cpp_name(owner)
        tag = self.write_tags('s_object_%s' % self.id(), definition, prefix)
        for alias, alias_cpp in self.all_names():
            definition.write('%sconst ::BugEngine::RTTI::ObjectInfo %ss%s_object_%s = {\n'
                             '    %s,\n'
                             '    %s,\n'
                             '    %s,\n'
                             '    ::BugEngine::RTTI::Value(%s, ::BugEngine::RTTI::Value::MakeConst)\n'
                             '};\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix,
                                       alias_cpp, object_name, tag, alias, n))
            object_name = '{&%ss%s_object_%s}' % (helper_name(struct_owners), prefix, alias_cpp)
        return object_name


class Container(CppObject):
    def __init__(self, name):
        super(Container, self).__init__()
        self.name = name
        self.objects = []
        self.anonymous = False

    def short_name(self):
        return self.name[-1]

    def set_visibility(self, visibility):
        pass

    def add_object(self, o):
        self.objects.append(o)

    def add_method(self, m):
        for alias, alias_cpp in m.all_names():
            for o in self.objects:
                if o.name == alias:
                    o.add_overload(m)
                    break
            else:
                self.objects.append(OverloadedMethod(m, alias, alias_cpp))

    def add_property(self, p):
        self.objects.append(p)


class Class(Container):
    def __init__(self, name, type, parents):
        super(Class, self).__init__(name)
        self.type = type
        self.published = type in ('enum', 'be_pod', 'struct', 'union')
        for visibility, parent in parents:
            if visibility in ('published', 'public') or (visibility == '' and self.published):
                self.parent = parent
                break
        else:
            if type == 'class':
                self.parent = 'void'
            else:
                self.parent = ''
        self.methods = []
        self.properties = []
        self.constructor = None
        if self.type == 'be_pod':
            self.add_constructor(PodConstructor(self))
            self.add_constructor(PodConstructorCopy(self))

    def owner_name(self):
        return '::BugEngine::be_typeid< ::%s >::klass()' % '::'.join(self.name)

    def cpp_name(self):
        return '::'.join(self.name)

    def helper_name(self):
        return '%s_BugHelper' % self.name[-1]

    def set_visibility(self, visibility):
        self.published = visibility == 'published'

    def add_method(self, method):
        if self.published:
            if isinstance(method, Constructor):
                self.add_constructor(method)
            elif 'static' in method.attributes:
                super(Class, self).add_method(method)
            else:
                for alias, alias_cpp in method.all_names():
                    for m in self.methods:
                        if m.name == alias:
                            m.add_overload(method)
                            break
                    else:
                        self.methods.append(OverloadedMethod(method, alias, alias_cpp))

    def add_property(self, p):
        if self.published:
            if 'static' in p.attributes:
                super(Class, self).add_property(p)
            else:
                self.properties.append(p)

    def add_object(self, o):
        if self.published:
            self.objects.append(o)

    def add_constructor(self, constructor):
        if not self.constructor:
            all_names = constructor.all_names()
            self.constructor = OverloadedMethod(constructor, all_names[0][0], all_names[0][1])
        else:
            self.constructor.add_overload(constructor)

    def do_typedef(self, names, definition):
        names = names + [self.name[-1]]
        definition.write('    typedef %s %s;\n' % ('::'.join(names), self.name[-1]))
        for o in self.objects:
            o.do_typedef(names, definition)

    def typedef(self, definition):
        for o in self.objects:
            o.do_typedef([self.name[-1]], definition)

    def declare_namespace(self, definition):
        pass

    def declare(self, owner, struct_owners, definition, instance, prefix):
        instance.write('raw< const ::BugEngine::RTTI::Class > klass_%s();\n' % self.name[-1])
        if struct_owners:
            for alias, alias_cpp in self.all_names():
                definition.write('    static const ::BugEngine::RTTI::ObjectInfo s%s_object_%s;\n' % (prefix, alias_cpp))

        definition.write('struct %s\n{\n\n' % self.helper_name())
        self.typedef(definition)
        prefix = ''
        struct_owners = struct_owners + [self]

        CppObject.declare(self, self, struct_owners, definition, instance, prefix)

        if self.type in ('enum'):
            definition.write('    static ::BugEngine::istring toString(%s v);\n' % self.cpp_name())
            definition.write('    static u32 toInt(%s v);\n' % self.cpp_name())
            m = BuiltinMethod("toString", "::BugEngine::istring",
                              [Parameter(self.cpp_name(), "this", None)], ['const'])
            self.add_method(m)
            m = BuiltinMethod("toInt", "u32",
                              [Parameter(self.cpp_name(), "this", None)], ['const'])
            self.add_method(m)

        if self.constructor:
            self.constructor.declare(self, struct_owners, definition, instance, prefix, True)
        for m in self.methods:
            m.declare(self, struct_owners, definition, instance, prefix, True)
        if self.methods or self.constructor:
            definition.write('    static const ::BugEngine::RTTI::Method s%s_methods[];\n' % prefix)
        if self.properties:
            for p in self.properties:
                p.declare(self, struct_owners, definition, instance, prefix)
            definition.write('    static const ::BugEngine::RTTI::Property s%s_properties[];\n' % prefix)
        for object in self.objects:
            object.declare(self, struct_owners, definition, instance, prefix)

        definition.write('};\n')

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        struct_owners = struct_owners + [self]
        prefix = ''
        helper = helper_name(struct_owners)
        params = {
            'NAME':         self.name[-1],
            'CPP_NAME':     self.cpp_name(),
            'PREFIX':       prefix,
            'HELPER_NAME':  helper,
            'OWNER':        owner.owner_name(),
            'NAMESPACE':    '::'.join(namespace),
        }
        if self.type in ('enum'):
            definition.write('::BugEngine::istring %stoString(%s v)\n'
                             '{\n'
                             '    %s\n'
                             '    %s\n'
                             '    return istring(minitl::format<64u>("Unknown(%%d)") | (u32)v);\n'
                             '}\n' % (helper, self.cpp_name(),
                                      '\n    '.join(('static const ::BugEngine::istring s_%s = "%s";' % (o.name, o.name) for o in self.objects[::-1])),
                                      '\n    '.join(('if (v == %s) return s_%s;' % (o.cpp_name(self), o.name) for o in self.objects[::-1]))
                                      )
                            )
            definition.write('u32 %(HELPER_NAME)stoInt(%(CPP_NAME)s v)\n'
                             '{\n'
                             '    return static_cast<u32>(v);\n'
                             '}\n\n' % params)

        for object in self.objects:
            object.write_content(self, struct_owners, prefix, namespace, definition, instance)

        for method in self.methods:
            method.write_content(self, struct_owners, prefix, namespace, definition, instance)

        if self.constructor:
            self.constructor.write_content(self, struct_owners, prefix, namespace, definition, instance)

        if self.type in ('struct', 'be_pod', 'enum', 'union'):
            params['COPYCONSTRUCTOR'] = '&::BugEngine::RTTI::wrapCopy< %(CPP_NAME)s >' % params
            params['DESTRUCTOR'] = '&::BugEngine::RTTI::wrapDestroy< %(CPP_NAME)s >' % params
        else:
            params['COPYCONSTRUCTOR'] = '0'
            params['DESTRUCTOR'] = '0'

        if self.parent:
            params['PARENT'] = '::BugEngine::be_typeid< %s >::klass()' % self.parent
            params['OFFSET'] = 'i32(ptrdiff_t(static_cast< %s* >((%s*)4))) - 4' % (self.name[-1], self.parent)
        else:
            params['PARENT'] = '::BugEngine::be_typeid< void >::klass()'
            params['OFFSET'] = '0'

        if self.parent:
            params['OBJECTS'] = '{::BugEngine::be_typeid< %s >::klass()->objects.m_ptr}' % self.parent
        else:
            params['OBJECTS'] = '{0}'

        for object in self.objects[::-1]:
            params['OBJECTS'] = object.write_object(self, struct_owners, prefix, namespace,
                                                    params['OBJECTS'], definition, instance)

        params['TAGS'] = self.write_tags(self.name[-1], definition, struct_owners)

        method_index = 0
        methods = []
        if self.constructor:
            overloads = self.constructor.write_overloads(self, struct_owners, prefix + '_method_%s' % self.constructor.name_cpp, definition)
            for name in self.constructor.all_names():
                methods.append((self.constructor, name[0], overloads, method_index))
                method_index += 1
        for m in self.methods:
            overloads = m.write_overloads(self, struct_owners, prefix + '_method_%s' % m.name_cpp, definition)
            for name in m.all_names():
                methods.append((m, name[0], overloads, method_index))
                method_index += 1
        if methods:
            definition.write('const ::BugEngine::RTTI::Method %ss%s_methods[%d] = {\n' % (helper_name(struct_owners), prefix, method_index))
            definition.write(',\n'.join(['    {\n'
                                         '        %s,\n'
                                         '        %s,\n'
                                         '        {&s%s_methods[%d]}\n'
                                         '    }' % (name, o, prefix, i) for m, name, o, i in methods]))
            definition.write('\n};\n\n')
            params['METHODS'] = '{ %d, %ss%s_methods }' % (method_index, helper_name(struct_owners), prefix)
        else:
            params['METHODS'] = '{ 0, 0 }'

        if self.constructor:
            params['CONSTRUCTOR'] = '{%ss_methods}' % helper_name(struct_owners)
        else:
            params['CONSTRUCTOR'] = '{0}'

        props = []
        for p in self.properties:
            tags = p.write_tags(struct_owners, definition, prefix + '_prop_' + p.name)
            for name in p.all_names():
                props.append((p, name[0], tags))
        if props:
            definition.write('const ::BugEngine::RTTI::Property %ss%s_properties[%d] = {\n'%(helper, prefix, len(props)))
            definition.write(',\n'.join(['    {\n'
                                         '        %s,\n'
                                         '        %s,\n'
                                         '        ::BugEngine::be_typeid< %s >::type(),\n'
                                         '        ::BugEngine::be_typeid< %s >::type(),\n'
                                         '        &::BugEngine::RTTI::PropertyHelper< %s, %s, &%s::%s >::get\n'
                                         '    }' % (t, name, self.cpp_name(), p.type,
                                                    p.type, self.cpp_name(), self.cpp_name(), p.name) for p, name, t in props]))
            definition.write('\n};\n\n')
            params['PROPERTIES'] = '{ %d, %ss%s_properties }' % (len(props), helper_name(struct_owners), prefix)
        else:
            params['PROPERTIES'] = '{ 0, 0 }'


        params['CLASSTYPE'] = self.index
        if params['CLASSTYPE'] is None:
            if self.type == 'enum':
                params['CLASSTYPE'] = '::BugEngine::RTTI::ClassType_Enum'
            elif self.type == 'class':
                params['CLASSTYPE'] = '::BugEngine::RTTI::ClassType_Object'
            elif self.type == 'struct':
                params['CLASSTYPE'] = '::BugEngine::RTTI::ClassType_Struct'
            elif self.type in ('be_pod', 'union'):
                params['CLASSTYPE'] = '::BugEngine::RTTI::ClassType_Pod'
            else:
                params['CLASSTYPE'] = '0'

        definition.write('raw< const ::BugEngine::RTTI::Class > klass_%s()\n'
                         '{\n' % (self.name[-1]))
        self.typedef(definition)
        definition.write('    static const\n'
                         '    ::BugEngine::RTTI::Class s%(PREFIX)s_class = {\n'
                         '        /* .name */               "%(NAME)s",\n'
                         '        /* .size */               u32(sizeof(%(CPP_NAME)s)),\n'
                         '        /* .offset */             %(OFFSET)s,\n'
                         '        /* .id */                 %(CLASSTYPE)s,\n'
                         '        /* .owner */              {%(OWNER)s.m_ptr},\n'
                         '        /* .parent */             {%(PARENT)s.m_ptr},\n'
                         '        /* .objects */            %(OBJECTS)s,\n'
                         '        /* .tags */               %(TAGS)s,\n'
                         '        /* .properties */         %(PROPERTIES)s,\n'
                         '        /* .methods */            %(METHODS)s,\n'
                         '        /* .constructor */        %(CONSTRUCTOR)s,\n'
                         '        /* .apiMethods */         {0},\n'
                         '        /* .copyconstructor */    %(COPYCONSTRUCTOR)s,\n'
                         '        /* .destructor */         %(DESTRUCTOR)s\n'
                         '    };\n\n'
                         '    raw< const ::BugEngine::RTTI::Class > result = { &s%(PREFIX)s_class };\n'
                         '    return result;\n'
                         '}\n\n'% params)


        instance.write('template< >\n'
                       'BE_EXPORT raw< const RTTI::Class > be_typeid< %(CPP_NAME)s >::klass()\n'
                       '{\n'
                       '    return %(NAMESPACE)s::klass_%(NAME)s();\n'
                       '}\n'
                       '\n'
                       'template< >\n'
                       'BE_EXPORT istring be_typeid< %(CPP_NAME)s >::name()\n'
                       '{\n'
                       '    static istring s_name("%(NAME)s");\n'
                       '    return s_name;\n'
                       '}\n'
                       '\n' % params)

    def write_object(self, owner, struct_owners, prefix, namespace, object_name, definition, instance):
        for alias, alias_cpp in self.all_names():
            definition.write('%sconst ::BugEngine::RTTI::ObjectInfo %ss%s_object_%s = {\n'
                             '    %s,\n'
                             '    {0},\n'
                             '    ::BugEngine::istring(%s),\n'
                             '    ::BugEngine::RTTI::Value(::BugEngine::be_typeid< %s >::klass())\n'
                             '};\n\n' % (helper_static(struct_owners), helper_name(struct_owners), prefix,
                                         alias_cpp, object_name, alias, self.cpp_name()))
            object_name = '{&%ss%s_object_%s}' % (helper_name(struct_owners), prefix, alias_cpp)
        return object_name


class Namespace(Container):
    def __init__(self, root_namespace, name):
        super(Namespace, self).__init__(name)
        self.root_namespace = root_namespace

    def owner_name(self):
        return '::BugEngine::be_%s_Namespace_%s()' % (self.root_namespace, '_'.join(self.name))

    def cpp_name(self):
        return '::'.join(self.name)

    def declare_namespace(self,  definition):
        definition.write('raw<RTTI::Class> '
                         'be_%s_Namespace_%s();\n' % (self.root_namespace, '_'.join(self.name)))
        for object in self.objects:
            object.declare_namespace(definition)

    def declare(self, owner, struct_owners, definition, instance, prefix):
        if self.objects:
            instance.write('namespace %s\n{\n\n' % self.name[-1])
            definition.write('namespace %s\n{\n\n' % self.name[-1])
            for object in self.objects:
                object.declare(self, struct_owners, definition, instance, prefix)
            definition.write('\n}\n')
            instance.write('\n}\n')

    def write_content(self, owner, struct_owners, prefix, namespace, definition, instance):
        if self.objects:
            definition.write('namespace %s\n{\n\n' % self.name[-1])
            next_object = first_object = '%s->objects' % self.owner_name()
            for object in self.objects:
                object.write_content(self, struct_owners, prefix, namespace + [self.name[-1]], definition, instance)
                next_object = object.write_object(self, struct_owners, prefix, namespace + [self.name[-1]],
                                                  next_object, definition, instance)
            if next_object != first_object:
                definition.write('static const raw<const ::BugEngine::RTTI::ObjectInfo> s_object_ptr_%s ='
                                 ' %s;\n'
                                 'BE_EXPORT const ::BugEngine::RTTI::ObjectInfo* s_object_set_%s ='
                                 ' %s->objects.set(s_object_ptr_%s.operator->());\n'
                                 '\n' % (object.id(), next_object, object.id(),
                                         self.owner_name(), object.id()))
            definition.write('\n}\n')

    def write_object(self, owner, struct_owner, prefix, namespace, object_name, definition, instance):
        return object_name


class AnonymousNamespace(Container):
    def __init__(self):
        super(AnonymousNamespace, self).__init__([])
        self.anonymous = True

    def add_object(self, o):
        pass

    def add_method(self, m):
        pass

    def add_property(self, p):
        pass


class AnonymousClass(Container):
    def __init__(self):
        super(AnonymousClass, self).__init__([])
        self.anonymous = True

    def add_object(self, o):
        pass

    def add_method(self, m):
        pass

    def add_property(self, p):
        pass

    def cpp_name(self):
        return '__anonymnous__'


class Root(Container):
    def __init__(self, root_namespace):
        super(Root, self).__init__([])
        self.root_namespace = root_namespace

    def owner_name(self):
        return '::BugEngine::be_%s_Namespace()' % self.root_namespace

    def cpp_name(self):
        return ''

    def dump(self, definition, instance, include):
        instance.write('#include <rtti/stdafx.h>\n')
        instance.write('#include <rtti/classinfo.script.hh>\n')
        instance.write('#include <rtti/typeinfo.hh>\n')
        instance.write('#include <%s>\n' % include)
        definition.write('#include <rtti/stdafx.h>\n')
        definition.write('#include <rtti/engine/helper/staticarray.factory.hh>\n')
        definition.write('#include <rtti/classinfo.script.hh>\n')
        definition.write('#include <rtti/typeinfo.hh>\n')
        definition.write('#include <rtti/engine/methodinfo.script.hh>\n')
        definition.write('#include <rtti/engine/objectinfo.script.hh>\n')
        definition.write('#include <rtti/engine/propertyinfo.script.hh>\n')
        definition.write('#include <rtti/engine/array.factory.hh>\n')
        definition.write('#include <rtti/engine/map.factory.hh>\n')
        definition.write('#include <rtti/engine/tuple.factory.hh>\n')
        definition.write('#include <rtti/engine/taginfo.script.hh>\n')
        definition.write('#include <rtti/engine/helper/method.hh>\n')
        definition.write('#include <rtti/engine/helper/get.hh>\n')
        definition.write('#include <%s>\n' % include)


        if self.objects:
            definition.write('\nnamespace BugEngine\n{\n')
            definition.write('raw<RTTI::Class> '
                             'be_%s_Namespace();\n' % (self.root_namespace))
            for object in self.objects:
                object.declare_namespace(definition)
            definition.write('}\n')
            for object in self.objects:
                object.declare(self, [], definition, instance, '')
            instance.write('\nnamespace BugEngine\n{\n')
            next_object = '%s->objects'%self.owner_name()
            for object in self.objects:
                object.write_content(self, [], '', [], definition, instance)
                next_object = object.write_object(self, [], '', [], next_object, definition, instance)
            instance.write('}\n\n')
        definition.write('\n\n')
