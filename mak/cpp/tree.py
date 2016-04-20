class CppObject(object):
    def __init__(self):
        self.tags = []

    def add_tag(self, tag):
        self.tags.append(tag)

    def add_tags(self, tags):
        self.tags += tags

    def get_tag_list(self, tag_name):
        result = []
        for type, value in self.tags:
            if type == tag_name:
                result.append(value)
        return result

    def all_names(self):
        result = [(self.engine_name(), self.id())]
        for alias in self.get_tag_list('Alias'):
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
        tag = '{0}'
        index = 0;
        for t in self.tags:
            if t[0] in ('Alias', 'Index'):
                continue
            definition.write('    static const ::BugEngine::RTTI::Tag s_tag_%s_%d = {\n'
                             '        %s,\n'
                             '        ::BugEngine::RTTI::Value(%s(%s))\n'
                             '    };\n' % (prefix, index, tag, t[0], t[1]))
            tag = '{&s_tag_%s_%d}' % (prefix, index)
            index += 1
        return tag

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

    def write(self, next_parameter, method_name, overload_index, definition):
        if self.type == '...':
            return (next_parameter[0]+1,
                    '{&s_parameter_%s_%d_%d}' % (method_name, overload_index, next_parameter[0]),
                    True)
        else:
            tag = self.write_tags('parameter_%s_%d_%d' % (method_name, overload_index,
                                                          next_parameter[0]),
                                  definition)
            definition.write('    static const ::BugEngine::RTTI::Method::Parameter '
                             's_parameter_%s_%d_%d = {\n'
                             '       %s,\n'
                             '       %s,\n'
                             '       ::BugEngine::istring("%s"),\n'
                             '       ::BugEngine::be_typeid< %s >::type()\n'
                             '    };\n' % (method_name, overload_index, next_parameter[0],
                                           tag, next_parameter[1], self.name, self.type))
            return (next_parameter[0]+1,
                    '{&s_parameter_%s_%d_%d}' % (method_name, overload_index, next_parameter[0]),
                    False)


class Method(CppObject):
    def __init__(self, name, return_type, parameters, attributes):
        super(Method, self).__init__()
        self.name = name
        self.return_type = return_type.strip()
        self.parameters = parameters
        self.attributes = attributes
        self.index = 0

    def add_attributes(self, attributes):
        self.attributes += attributes

    def cpp_name(self, owner, struct_owner):
        if struct_owner:
            return '%s::%s' % (owner.cpp_name(), self.name)
        else:
            return self.name

    def trampoline_name(self, owner):
        return 'trampoline_%s_%s_%d' % (owner.name[-1], self.name, self.index)

    def extra_params(self, owner, struct_owner):
        return []

    def call(self, owner, struct_owner):
        if struct_owner and not 'static' in self.attributes:
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
        definition.write('static ::BugEngine::RTTI::Value %s'
                         '(::BugEngine::RTTI::Value* parameters, u32 parameterCount)\n'
                         '{\n'
                         '    be_forceuse(parameters);\n'
                         '    be_forceuse(parameterCount);\n'
                          % self.trampoline_name(owner))
        if struct_owner:
            struct_owner.typedef(definition)
        if self.return_type != 'void':
            definition.write('    return ::BugEngine::RTTI::Value(%s);\n'
                             '}\n' % self.call(owner, struct_owner))
        else:
            definition.write('    %s;\n'
                             '    return ::BugEngine::RTTI::Value();\n'
                             '}\n' % self.call(owner, struct_owner))

    def write_parameters(self, owner, struct_owner, overload_index, definition):
        next_parameter = (0, '{0}', False)
        param_list = self.parameters
        if struct_owner and not 'static' in self.attributes:
            if 'const' in self.attributes:
                param_list.insert(0, Parameter('const %s&'%owner.cpp_name(), 'this', None))
            else:
                param_list.insert(0, Parameter('%s&'%owner.cpp_name(), 'this', None))
        for p in param_list[::-1]:
            next_parameter = p.write(next_parameter, self.name, overload_index, definition)
        return next_parameter

    def write(self, overload, owner, struct_owner, definition):
        tag = self.write_tags('overload_%s_%d' % (self.name, overload[0]), definition)
        params = self.write_parameters(owner, struct_owner, overload[0], definition)
        definition.write('    static const ::BugEngine::RTTI::Method::Overload s_overload_%s_%d = {\n'
                         '       %s,\n'
                         '       %s,\n'
                         '       ::BugEngine::be_typeid< %s >::type(),\n'
                         '       %d,\n'
                         '       %s,\n'
                         '       %s,\n'
                         '       &%s\n'
                         '    };\n' % (self.name, overload[0], tag, overload[1], self.return_type,
                                       params[0], params[1], params[2] and 'true' or 'false',
                                       self.trampoline_name(owner)))
        return (overload[0]+1, '{&s_overload_%s_%d}' % (self.name, overload[0]))


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

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        overload = (0, '{0}')
        for o in self.overloads[::-1]:
            overload = o.write(overload, owner, struct_owner, definition)
        definition.write('    static const ::BugEngine::RTTI::Method s_method_%s = {\n'
                         '       %s,\n'
                         '       {0},\n'
                         '       {&s_method_%s},\n'
                         '       %d,\n'
                         '       %s\n'
                         '    };\n'
                         '    static const raw<const ::BugEngine::RTTI::Method> s_method_ptr_%s = '
                         '{&s_method_%s};\n'
                         '    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                         '        %s,\n'
                         '        {0},\n'
                         '        s_method_%s.name,\n'
                         '        ::BugEngine::RTTI::Value(s_method_ptr_%s)\n'
                         '    };\n' % (self.name_cpp, self.name, self.name_cpp,
                                       overload[0], overload[1], self.name_cpp, self.name_cpp,
                                       self.name_cpp, object_name,
                                       self.name_cpp, self.name_cpp))
        return '{&s_object_%s}' % self.name_cpp

    def write_method(self, owner, method, definition):
        overload = (0, '{0}')
        for o in self.overloads[::-1]:
            overload = o.write(overload, owner, owner, definition)
        definition.write('    static const ::BugEngine::RTTI::Method s_method_%s = {\n'
                         '       %s,\n'
                         '       %s,\n'
                         '       {&s_method_%s},\n'
                         '       %d,\n'
                         '       %s\n'
                         '    };\n' % (self.name_cpp, self.name, method,
                                       self.name_cpp, overload[0], overload[1]))
        return '{&s_method_%s}' % self.name_cpp


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
            definition.write('    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                             '        %s,\n'
                             '        %s,\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(\n'
                             '            ::BugEngine::RTTI::Value::ByRef(%s::%s))\n'
                             '    };\n' % (alias_cpp, object_name, tag,
                                           alias, owner.cpp_name(), self.name))
            object_name = '{&s_object_%s}' % alias_cpp
        return object_name

    def write_property(self, owner, property_name, definition):
        tag = self.write_tags('s_property_%s' % self.id(), definition)
        for alias, alias_cpp in self.all_names():
            definition.write('    static ::BugEngine::RTTI::Property s_property_%s = {\n'
                             '        %s,\n'
                             '        %s,\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::be_typeid< %s >::type(),\n'
                             '        ::BugEngine::be_typeid< %s >::type(),\n'
                             '        &::BugEngine::RTTI::PropertyHelper< %s, %s, &%s::%s >::get\n'
                             '    };\n' % (alias_cpp, tag, property_name,
                                           alias, owner.cpp_name(), self.type,
                                           self.type, owner.cpp_name(), owner.cpp_name(), self.name))
            property_name = '{&s_property_%s}' % alias_cpp
        return property_name


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
            definition.write('    static ::BugEngine::RTTI::ObjectInfo s_object_td_%s = {\n'
                             '        %s,\n'
                             '        %s,\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(\n'
                             '            ::BugEngine::be_typeid< %s >::type())\n'
                             '    };\n' % (alias_cpp, object_name, tag,
                                           alias, self.name))
            object_name = '{&s_object_td_%s}' % alias_cpp
        return object_name


class EnumValue(Variable):
    def __init__(self, name, type):
        super(EnumValue, self).__init__(name, type)
        self.add_attributes(['static'])

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        n = '%s::%s' % ('::'.join(owner.cpp_name().split('::')[:-1]), self.name)
        tag = self.write_tags('s_object_%s' % self.id(), definition)
        for alias, alias_cpp in self.all_names():
            definition.write('    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
                             '        %s,\n'
                             '        %s,\n'
                             '        ::BugEngine::istring(%s),\n'
                             '        ::BugEngine::RTTI::Value(\n'
                             '            ::BugEngine::RTTI::Value::ByRef(%s))\n'
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
        for object in self.objects:
            object.write_content(self, struct_owner or self, namespace, definition, instance)
        for method in self.methods:
            method.write_content(self, struct_owner or self, namespace, definition, instance)
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
            methods = '::BugEngine::be_typeid< %s >::klass()->methods'
            properties = '::BugEngine::be_typeid< %s >::klass()->properties'
        else:
            parent = '::BugEngine::be_typeid< void >::preklass()'
            offset = '0'
            methods = '{0}'
            properties = '{0}'
        definition.write('raw< ::BugEngine::RTTI::Class > preklass_%s()\n'
                         '{\n' % self.name[-1])
        if struct_owner:
            struct_owner.typedef(definition)
        else:
            self.typedef(definition)
        tag = self.write_tags('cls', definition)
        try:
            classtype = self.get_tag_list('Index')[-1]
        except IndexError:
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
        definition.write('    static ::BugEngine::RTTI::Class cls = {\n'
                         '        ::BugEngine::istring("%s"),\n'
                         '        %s,\n'
                         '        %s,\n'
                         '        u32(sizeof(%s)),\n'
                         '        %s,\n'
                         '        %s,\n'
                         '        %s,\n'
                         '        {0},\n'
                         '        {0},\n'
                         '        {0},\n'
                         '        {0},\n'
                         '        {0},\n'
                         '        %s,\n'
                         '        %s};\n'
                         '    raw< ::BugEngine::RTTI::Class > result = { &cls };\n'
                         '    return result;\n'
                         '}\n'% (self.name[-1], owner.owner_name(), parent, self.cpp_name(),
                                 offset, classtype, tag, copy, destructor))
        definition.write('raw< const ::BugEngine::RTTI::Class > properties_%s()\n'
                         '{\n' % self.name[-1])
        if struct_owner:
            struct_owner.typedef(definition)
        else:
            self.typedef(definition)
        if self.parent:
            next_object = '::BugEngine::be_typeid< %s >::klass()->objects' % self.parent
            next_method = '::BugEngine::be_typeid< %s >::klass()->methods' % self.parent
            next_property = '::BugEngine::be_typeid< %s >::klass()->properties' % self.parent
        else:
            next_object = '{0}'
            next_method = '{0}'
            next_property = '{0}'
        if self.constructor:
            constructor = self.constructor.write_method(self, next_method, definition)
        else:
            constructor = '{0}'
        definition.write('    raw< ::BugEngine::RTTI::Class > result = '
                         'preklass_%s();\n' % self.name[-1])
        for object in self.objects[::-1]:
            next_object = object.write_object(self, struct_owner or self, namespace,
                                              next_object, definition, instance)
            if object is Constructor:
                constructor = next_object
        for method in self.methods[::-1]:
            next_method = method.write_method(self, next_method, definition)
        for property in self.properties[::-1]:
            next_property = property.write_property(self, next_property, definition)
        definition.write('    raw<const ::BugEngine::RTTI::ObjectInfo> objects = %s;\n'
                         '    result->objects.set(objects.operator->());\n'
                         '    raw<const ::BugEngine::RTTI::Method> methods = %s;\n'
                         '    result->methods.set(methods.operator->());\n'
                         '    raw<const ::BugEngine::RTTI::Method> constructor = %s;\n'
                         '    result->constructor.set(constructor.operator->());\n'
                         '    raw<const ::BugEngine::RTTI::Property> properties = %s;\n'
                         '    result->properties.set(properties.operator->());\n'
                         '    return result;\n'
                         '}\n' % (next_object, next_method, constructor, next_property))
        definition.write('BE_EXPORT raw<const ::BugEngine::RTTI::Class> s_create_%s = '
                         '::BugEngine::be_typeid< %s >::klass();\n' % (self.name[-1],
                                                                       self.cpp_name()))
        instance.write('template< >\n'
                       'BE_EXPORT raw< RTTI::Class > be_typeid< %s >::preklass()\n'
                       '{\n'
                       '    return %s::preklass_%s();\n'
                       '}\n'
                       '\n' % (self.cpp_name(), '::'.join(namespace), self.name[-1]))
        instance.write('template< >\n'
                       'BE_EXPORT raw< const RTTI::Class > be_typeid< %s >::registerProperties()\n'
                       '{\n'
                       '    return %s::properties_%s();\n'
                       '}\n'
                       '\n' % (self.cpp_name(), '::'.join(namespace), self.name[-1]))

    def write_object(self, owner, struct_owner, namespace, object_name, definition, instance):
        for alias, alias_cpp in self.all_names():
            definition.write('    static ::BugEngine::RTTI::ObjectInfo s_object_%s = {\n'
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
                definition.write('static raw<const ::BugEngine::RTTI::ObjectInfo> s_object_ptr_%s ='
                                 ' %s;\n'
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
        definition.write('#include <rtti/engine/array.hh>\n')
        definition.write('#include <rtti/engine/taginfo.script.hh>\n')
        definition.write('#include <rtti/engine/helper/method.hh>\n')
        definition.write('#include <rtti/engine/helper/get.hh>\n')
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
