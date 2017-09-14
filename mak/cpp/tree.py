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

    def declare(self, definition, instance):
        pass

    def write_tags(self, prefix, definition):
        if self.tags:
            definition.write('    be_section(rtti_tag)\n'
                             '    static ::BugEngine::RTTI::staticarray_n< %d, const ::BugEngine::RTTI::Tag > s_tags_%s = {\n'
                             '        %d,\n'
                             '        {\n'  % (len(self.tags), prefix, len(self.tags)))
            definition.write(',\n'.join(['            { ::BugEngine::RTTI::Value(%s(%s)) }' % (t[0], t[1]) for t in self.tags]))
            definition.write('\n        }\n    };\n')
            return 'reinterpret_cast< ::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Tag>* >(&s_tags_%s)' % prefix
        else:
            return '::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Tag>::s_null'

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        return object_name


class Parameter(CppObject):
    def __init__(self, type, name, default_value):
        super(Parameter, self).__init__()
        self.type = type
        self.name = name
        self.default_value = default_value

    def write_tags(self, method_name, overload_index, param_index, definition):
        return super(Parameter, self).write_tags('parameter_%s_%d_%s' % (method_name, overload_index, param_index),
                                                 definition)


class Method(CppObject):
    def __init__(self, name, return_type, parameters, attributes):
        super(Method, self).__init__()
        self.name = name
        self.return_type = return_type.strip()
        self.parameters = parameters[:]
        self.attributes = attributes
        self.index = 0
        self.vararg = self.parameters and self.parameters[-1].type == '...'
        if self.vararg:
            self.parameters.pop(-1)

    def add_attributes(self, attributes):
        self.attributes += attributes

    def cpp_name(self, owner, struct_owner):
        if struct_owner:
            return '%s::%s' % (owner.cpp_name(), self.name)
        else:
            return self.name

    def trampoline_name(self, owner):
        if owner.name:
            return 'trampoline_%s_%s_%d' % (owner.name[-1], self.name, self.index)
        else:
            return 'trampoline_%s_%d' % (self.name, self.index)

    def extra_params(self, owner, struct_owner):
        return []

    def call(self, owner, struct_owner):
        if struct_owner and not 'static' in self.attributes and not 'builtin' in self.attributes:
            params = ', '.join(self.extra_params(owner, struct_owner) +
                               ['parameters[%d].as< %s >()' % (i+1, p.type)
                                for i, p in enumerate(self.parameters)])
            if 'const' in self.attributes:
                return 'parameters[0].as< const %s& >().%s(%s)' % (owner.cpp_name(),
                                                                   self.cpp_name(owner,
                                                                                 struct_owner),
                                                                   params)
            else:
                return 'parameters[0].as< %s& >().%s(%s)' % (owner.cpp_name(),
                                                             self.cpp_name(owner, struct_owner),
                                                             params)
        else:
            params = ', '.join(self.extra_params(owner, struct_owner) +
                               ['parameters[%d].as< %s >()' % (i, p.type)
                                for i, p in enumerate(self.parameters)])
            return '%s(%s)' % (self.cpp_name(owner, struct_owner), params)

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        definition.write('be_section(rtti_text_trampoline)\n'
                         'static ::BugEngine::RTTI::Value %s'
                         '(::BugEngine::RTTI::Value* parameters, u32 parameterCount)\n'
                         '{\n'
                         '    be_forceuse(parameters);\n'
                         '    be_forceuse(parameterCount);\n'
                          % self.trampoline_name(owner))
        if struct_owner:
            struct_owner.typedef(definition)
        if self.return_type != 'void':
            if self.return_type[-1] == '&':
                definition.write('    return ::BugEngine::RTTI::Value(::BugEngine::RTTI::Value::ByRef(%s));\n'
                                 '}\n' % self.call(owner, struct_owner))
            else:
                definition.write('    return ::BugEngine::RTTI::Value(%s);\n'
                                 '}\n' % self.call(owner, struct_owner))
        else:
            definition.write('    %s;\n'
                             '    return ::BugEngine::RTTI::Value();\n'
                             '}\n' % self.call(owner, struct_owner))

    def write_parameters(self, owner, struct_owner, overload_index, definition):
        params = []
        param_index = 0
        if struct_owner and not 'static' in self.attributes and not 'builtin' in self.attributes:
            param_index = param_index + 1
            if 'const' in self.attributes:
                params.append((Parameter('const %s&'%owner.cpp_name(), 'this', None), '0'))
            else:
                params.append((Parameter('%s&'%owner.cpp_name(), 'this', None), '0'))
        for p in self.parameters:
            param_index = param_index + 1
            params.append((p, p.write_tags(self.name, overload_index, param_index, definition)))

        if params:
            definition.write('    be_section(rtti_method)\n'
                             '    static ::BugEngine::RTTI::staticarray_n< %d, const ::BugEngine::RTTI::Method::Parameter > s_%s_%d_params = {\n'
                             '        %d,\n'
                             '        {\n'  % (len(params), self.name, overload_index, len(params)))
            definition.write(',\n'.join(['            {\n'
                                         '                {%s},\n'
                                         '                ::BugEngine::istring("%s"),\n'
                                         '                ::BugEngine::be_typeid< %s >::type(),\n'
                                         '                ::BugEngine::RTTI::Value(%s)\n'
                                         '            }' % (t, p.name, p.type,
                                                            p.default_value and ('%s(%s)'%(p.type, p.default_value)) or '') for p, t in params]))
            definition.write('\n        }\n    };\n')
            return 'reinterpret_cast< ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Method::Parameter >* >(&s_%s_%d_params)' % (self.name, overload_index)
        else:
            return '::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Method::Parameter>::s_null'


class BuiltinMethod(Method):
    def __init__(self, name, return_type, parameters, attributes):
        super(BuiltinMethod, self).__init__(name, return_type, parameters, attributes + ['builtin'])

    def cpp_name(self, owner, struct_owner):
        return self.name


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

    def cpp_name(self, owner, struct_owner):
        if struct_owner:
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

    def cpp_name(self, owner, struct_owner):
        if owner.type == 'class':
            return 'ref< %s >::create' % (owner.cpp_name())
        else:
            return self.name

    def engine_name(self):
        return '::BugEngine::RTTI::Class::nameConstructor()'

    def extra_params(self, owner, struct_owner):
        if owner.type == 'class':
            return ['::BugEngine::Arena::general()']
        else:
            return []


class PodConstructor(Constructor):
    def __init__(self, owner):
        super(PodConstructor, self).__init__(owner, owner.name[-1], owner.name[-1], [], [])

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        pass

    def trampoline_name(self, owner):
        return '::BugEngine::RTTI::createPod< %s >' % owner.cpp_name()



class PodConstructorCopy(Constructor):
    def __init__(self, owner):
        super(PodConstructorCopy, self).__init__(owner, owner.name[-1], owner.name[-1],
                                                 [Parameter('const %s&' % owner.cpp_name(), 'other', None)], [])

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        pass

    def trampoline_name(self, owner):
        return '::BugEngine::RTTI::createPodCopy< %s >' % owner.cpp_name()


class Destructor(Method):
    def __init__(self, name, return_type, parameters, attributes):
        super(Destructor, self).__init__('destroy_%s' % name.strip(), return_type, parameters,
                                         attributes)

    def cpp_name(self, owner, struct_owner):
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

    def add_overload(self, overload):
        overload.index = len(self.overloads)
        self.overloads.append(overload)

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        for o in self.overloads:
            o.write_content(owner, struct_owner, namespace, definition, instance)

    def write_overloads(self, owner, struct_owner, definition):
        overloads = []
        overload_index = 0
        for o in self.overloads[::-1]:
            overloads.append((o,
                              o.write_tags('method_%s_%d' % (self.name_cpp, overload_index), definition),
                              o.write_parameters(owner, struct_owner, overload_index, definition)))
            overload_index += 1
        definition.write('    be_section(rtti_method)\n'
                         '    static ::BugEngine::RTTI::staticarray_n< %d, const ::BugEngine::RTTI::Method::Overload > s_method_%s_overloads = {\n'
                         '        %d,\n'
                         '        {\n' %(len(overloads), self.name_cpp, len(overloads)))
        definition.write(',\n'.join(['            {\n'
                                     '                {%s},\n'
                                     '                {%s},\n'
                                     '                ::BugEngine::be_typeid< %s >::type(),\n'
                                     '                %s,\n'
                                     '                &%s\n'
                                     '            }' % (t, p, o.return_type, o.vararg and "true" or "false", o.trampoline_name(owner)) for o, t, p in overloads]))
        definition.write('\n        }\n    };\n')
        return 'reinterpret_cast< ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Method::Overload >* >(&s_method_%s_overloads)' % (self.name_cpp)

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        method_ptr = self.write_method(owner, struct_owner, definition)
        definition.write('    be_section(rtti_method)\n'
                         '    static raw<const ::BugEngine::RTTI::Method> s_method_ptr_%s = '
                         '{&%s};\n'
                         '    be_section(rtti_object)\n'
                         '    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                         '        %s,\n'
                         '        {::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Tag>::s_null},\n'
                         '        s_method_%s.name,\n'
                         '        ::BugEngine::RTTI::Value(s_method_ptr_%s)\n'
                         '    };\n' % (self.name_cpp, method_ptr,
                                       self.name_cpp, object_name, self.name_cpp, self.name_cpp))
        return '{&s_object_%s}' % self.name_cpp

    def write_method(self, owner, struct_owner, definition):
        overload_ptr = self.write_overloads(owner, struct_owner, definition)
        definition.write('    be_section(rtti_method)\n'
                         '    static ::BugEngine::RTTI::Method s_method_%s  = {\n'
                         '       %s,\n'
                         '       {%s},\n'
                         '       {&s_method_%s}\n'
                         '    };\n' % (self.name_cpp, self.name, overload_ptr, self.name_cpp))
        return 's_method_%s' % self.name_cpp


class Variable(CppObject):
    def __init__(self, name, type):
        super(Variable, self).__init__()
        self.name = name
        self.type = type
        self.attributes = []

    def add_attributes(self, attributes):
        self.attributes += attributes

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        tag = self.write_tags('s_object_%s' % self.id(), definition)
        for alias, alias_cpp in self.all_names():
            definition.write('    be_section(rtti_object)\n'
                             '    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                             '        %s,\n'
                             '        {%s},\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(\n'
                             '            ::BugEngine::RTTI::Value::ByRef(%s::%s))\n'
                             '    };\n' % (alias_cpp, object_name, tag,
                                           alias, owner.cpp_name(), self.name))
            object_name = '{&s_object_%s}' % alias_cpp
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

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        pass

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        tag = self.write_tags('s_object_%s' % self.id(), definition)
        for alias, alias_cpp in self.all_names():
            definition.write('    be_section(rtti_object)\n'
                             '    static ::BugEngine::RTTI::ObjectInfo s_object_td_%s = {\n'
                             '        %s,\n'
                             '        {%s},\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(\n'
                             '            ::BugEngine::be_typeid< %s >::type())\n'
                             '    };\n' % (alias_cpp, object_name, tag,
                                           alias, self.name))
            object_name = '{&s_object_td_%s}' % alias_cpp
        return object_name


class EnumValue(Variable):
    def __init__(self, name, value, type):
        super(EnumValue, self).__init__(name, type)
        self.value = value
        self.add_attributes(['static'])

    def cpp_name(self, owner):
        return '%s::%s' % ('::'.join(owner.cpp_name().split('::')[:-1]), self.name)

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        n = self.cpp_name(owner)
        tag = self.write_tags('s_object_%s' % self.id(), definition)
        for alias, alias_cpp in self.all_names():
            definition.write('    be_section(rtti_object)\n'
                             '    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                             '        %s,\n'
                             '        {%s},\n'
                             '        %s,\n'
                             '        ::BugEngine::RTTI::Value(%s, ::BugEngine::RTTI::Value::MakeConst)\n'
                             '    };\n' % (alias_cpp, object_name, tag,
                                           alias, n))
            object_name = '{&s_object_%s}' % alias_cpp
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
        return '::BugEngine::be_typeid< ::%s >::preklass()' % '::'.join(self.name)

    def cpp_name(self):
        return '::'.join(self.name)

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

    def declare(self, definition, instance):
        instance.write('raw< ::BugEngine::RTTI::Class > preklass_%s();\n' % self.name[-1])
        instance.write('raw< const ::BugEngine::RTTI::Class > properties_%s();\n' % self.name[-1])
        for object in self.objects:
            object.declare(definition, instance)

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        if self.type in ('enum'):
            definition.write('    be_section(rtti_text_enum)\n'
                             '    static ::BugEngine::istring toString(%s v)\n'
                             '    {\n'
                             '        %s\n'
                             '        %s\n'
                             '        return istring(minitl::format<64u>("Unknown(%%d)") | (u32)v);\n'
                             '    }\n' % (self.cpp_name(),
                                          '\n        '.join(('static const istring s_%s = "%s";' % (o.name, o.name) for o in self.objects[::-1])),
                                          '\n        '.join(('if (v == %s) return s_%s;' % (o.cpp_name(self), o.name) for o in self.objects[::-1]))
                                          )
                            )
            definition.write('    be_section(rtti_text_enum)\n'
                             '    static u32 toInt(%s v)\n'
                             '    {\n'
                             '        return static_cast<u32>(v);\n'
                             '    }\n' % (self.cpp_name())
                            )
            m = BuiltinMethod("toString", "::BugEngine::istring",
                              [Parameter(self.cpp_name(), "this", None)], ['const'])
            #m.add_tag(('Alias', '"toString"'))
            self.add_method(m)
            m = BuiltinMethod("toInt", "u32",
                              [Parameter(self.cpp_name(), "this", None)], ['const'])
            #m.add_tag(('Alias', '"toInt"'))
            self.add_method(m)
        for object in self.objects:
            object.write_content(self, struct_owner or self, namespace, definition, instance)
        for method in self.methods:
            method.write_content(self, self, namespace, definition, instance)
        if self.constructor:
            self.constructor.write_content(self, struct_owner or self, namespace, definition, instance)
        if self.type in ('struct', 'be_pod', 'enum', 'union'):
            copy       = '&::BugEngine::RTTI::wrapCopy< %s >' % self.name[-1]
            destructor = '&::BugEngine::RTTI::wrapDestroy< %s >' % self.name[-1]
        else:
            copy = '0'
            destructor = '0'
        if self.parent:
            parent = '::BugEngine::be_typeid< %s >::preklass()' % self.parent
            offset = 'i32(ptrdiff_t(static_cast< %s* >((%s*)4))) - 4' % (self.name[-1], self.parent)
        else:
            parent = '::BugEngine::be_typeid< void >::preklass()'
            offset = '0'
        definition.write('be_section(rtti_text_cls)\n'
                         'raw< ::BugEngine::RTTI::Class > preklass_%s()\n'
                         '{\n' % self.name[-1])
        if struct_owner:
            struct_owner.typedef(definition)
        else:
            self.typedef(definition)
        classtype = self.index
        if classtype is None:
            if self.type == 'enum':
                classtype = '::BugEngine::RTTI::ClassType_Enum'
            elif self.type == 'class':
                classtype = '::BugEngine::RTTI::ClassType_Object'
            elif self.type == 'struct':
                classtype = '::BugEngine::RTTI::ClassType_Struct'
            elif self.type in ('be_pod', 'union'):
                classtype = '::BugEngine::RTTI::ClassType_Pod'
            else:
                classtype = '0'

        definition.write('    be_section(rtti_cls)\n'
                         '    static ::BugEngine::RTTI::Class cls = {\n'
                         '        ::BugEngine::be_typeid< %s >::name(),\n'
                         '        u32(sizeof(%s)),\n'
                         '        %s,\n'
                         '        %s,\n'
                         '        {%s.m_ptr},\n'
                         '        {%s.m_ptr},\n'
                         '        {0},\n'
                         '        {::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Tag>::s_null},\n'
                         '        {::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Property>::s_null},\n'
                         '        {::BugEngine::RTTI::staticarray<const ::BugEngine::RTTI::Method>::s_null},\n'
                         '        {0},\n'
                         '        {0},\n'
                         '        %s,\n'
                         '        %s};\n'
                         '    raw< ::BugEngine::RTTI::Class > result = { &cls };\n'
                         '    return result;\n'
                         '}\n\n'% (self.cpp_name(), self.cpp_name(), offset, classtype,
                                   owner.owner_name(), parent, copy, destructor))

        definition.write('be_section(rtti_text_cls_props)\n'
                         'raw< const ::BugEngine::RTTI::Class > properties_%s() \n'
                         '{\n' % self.name[-1])
        if struct_owner:
            struct_owner.typedef(definition)
        else:
            self.typedef(definition)

        definition.write('    raw< ::BugEngine::RTTI::Class > result = preklass_%s();\n' % self.name[-1])

        if self.parent:
            next_object = '{::BugEngine::be_typeid< %s >::klass()->objects.m_ptr}' % self.parent
        else:
            next_object = '{0}'
        for object in self.objects[::-1]:
            next_object = object.write_object(self, struct_owner or self, namespace,
                                              next_object, definition, instance)

        if self.tags:
            tags = self.write_tags(self.name[-1], definition)
            definition.write('    result->tags.set(&%s);\n' % (tags))

        method_index = 0
        methods = []
        if self.constructor:
            overloads = self.constructor.write_overloads(self, struct_owner or self, definition)
            for name in self.constructor.all_names():
                methods.append((self.constructor, name[0], overloads, method_index))
                method_index += 1
        for m in self.methods[::-1]:
            overloads = m.write_overloads(self, struct_owner or self, definition)
            for name in m.all_names():
                methods.append((m, name[0], overloads, method_index))
                method_index += 1
        if methods:
            definition.write('    be_section(rtti_method)\n'
                             '    static ::BugEngine::RTTI::staticarray_n< %d, const ::BugEngine::RTTI::Method > s_methods = {\n'
                             '        %d,\n'
                             '        {\n'  % (len(methods), len(methods)))
            definition.write(',\n'.join(['            {\n'
                                         '                %s,\n'
                                         '                {%s},\n'
                                         '                {&s_methods.elements[%d]}\n'
                                         '            }' % (name, o, i) for m, name, o, i in methods]))
            definition.write('\n        }\n    };\n'
                             '    result->methods.set(reinterpret_cast< ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Method >* >(&s_methods));\n')
        if self.constructor:
            definition.write('    result->constructor.set(result->methods->elements);\n')
        props = []
        for p in self.properties[::-1]:
            tags = p.write_tags(struct_owner or self, definition)
            for name in p.all_names():
                props.append((p, name[0], tags))
        if props:
            definition.write('    be_section(rtti_prop)\n'
                             '    static ::BugEngine::RTTI::staticarray_n< %d, const ::BugEngine::RTTI::Property > s_properties = {\n'
                             '        %d,\n'
                             '        {\n' % (len(props), len(props)))
            definition.write(',\n'.join(['            {\n'
                                         '                {%s},\n'
                                         '                %s,\n'
                                         '                ::BugEngine::be_typeid< %s >::type(),\n'
                                         '                ::BugEngine::be_typeid< %s >::type(),\n'
                                         '                &::BugEngine::RTTI::PropertyHelper< %s, %s, &%s::%s >::get\n'
                                         '            }' % (t, name, self.cpp_name(), p.type,
                                                            p.type, self.cpp_name(), self.cpp_name(), p.name) for p, name, t in props]))
            definition.write('\n        }\n    };\n'
                             '    result->properties.set(reinterpret_cast< ::BugEngine::RTTI::staticarray< const ::BugEngine::RTTI::Property >* >(&s_properties));\n')

        definition.write('    raw<const ::BugEngine::RTTI::ObjectInfo> objects = %s;\n'
                         '    result->objects.set(objects.operator->());\n'
                         '    return result;\n'
                         '}\n\n' % (next_object))
        definition.write('BE_EXPORT\n'
                         'raw<const ::BugEngine::RTTI::Class> s_create_%s = '
                         '::BugEngine::be_typeid< %s >::klass();\n'
                         '\n' % (self.name[-1], self.cpp_name()))
        instance.write('template< >\n'
                       'BE_EXPORT raw< RTTI::Class > be_typeid< %s >::preklass()\n'
                       '{\n'
                       '    return %s::preklass_%s();\n'
                       '}\n'
                       '\n' % (self.cpp_name(), '::'.join(namespace), self.name[-1]))
        instance.write('template< >\n'
                       'BE_EXPORT raw< const RTTI::Class > be_typeid< %s >::registerProperties()\n'
                       '{\n'
                       '    static raw< const RTTI::Class > result = %s::properties_%s();\n'
                       '    return result;\n'
                       '}\n'
                       '\n' % (self.cpp_name(), '::'.join(namespace), self.name[-1]))
        instance.write('template< >\n'
                       'BE_EXPORT istring be_typeid< %s >::name()\n'
                       '{\n'
                       '    static const istring s_name = "%s";\n'
                       '    return s_name;\n'
                       '}\n'
                       '\n' % (self.cpp_name(), self.name[-1]))

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        for alias, alias_cpp in self.all_names():
            definition.write('    be_section(rtti_object)\n'
                             '    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                             '        %s,\n'
                             '        {0},\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(::BugEngine::be_typeid< %s >::klass())\n'
                             '    };\n' % (alias_cpp, object_name, alias, self.cpp_name()))
            object_name = '{&s_object_%s}' % alias_cpp
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

    def declare(self,  definition, instance):
        if self.objects:
            instance.write('namespace %s\n{\n\n' % self.name[-1])
            for object in self.objects:
                object.declare(definition, instance)
            instance.write('\n}\n')

    def write_content(self, owner, struct_owner, namespace, definition, instance):
        if self.objects:
            definition.write('namespace %s\n{\n\n' % self.name[-1])
            next_object = first_object = '%s->objects' % self.owner_name()
            for object in self.objects:
                object.write_content(self, None, namespace + [self.name[-1]], definition, instance)
                next_object = object.write_object(self, None, namespace + [self.name[-1]],
                                                  next_object, definition, instance)
            if next_object != first_object:
                definition.write('be_section(rtti_object)\n'
                                 'static raw<const ::BugEngine::RTTI::ObjectInfo> s_object_ptr_%s ='
                                 ' %s;\n'
                                 'be_section(rtti_object)\n'
                                 'BE_EXPORT const ::BugEngine::RTTI::ObjectInfo* s_object_set_%s ='
                                 ' %s->objects.set(s_object_ptr_%s.operator->());\n'
                                 '\n' % (object.id(), next_object, object.id(),
                                         self.owner_name(), object.id()))
            definition.write('\n}\n')

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
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

    def dump(self, definition, instance):
        instance.write('#include <rtti/stdafx.h>\n')
        instance.write('#include <rtti/classinfo.script.hh>\n')
        instance.write('#include <rtti/typeinfo.hh>\n')
        definition.write('#include <rtti/stdafx.h>\n')
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
        definition.write('#include <rtti/engine/helper/staticarray.factory.hh>\n')

        if self.objects:
            definition.write('\nnamespace BugEngine\n{\n')
            for object in self.objects:
                object.declare_namespace(definition)
            definition.write('}\n')
            for object in self.objects:
                object.declare(definition, instance)
            instance.write('\nnamespace BugEngine\n{\n')
            next_object = '%s->objects'%self.owner_name()
            for object in self.objects:
                object.write_content(self, None, [], definition, instance)
                next_object = object.write_object(self, None, [], next_object, definition, instance)
            instance.write('}\n')
