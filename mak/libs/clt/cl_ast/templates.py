from .cppobject import CppObject
from .name import Name
try:
    from itertools import zip_longest
except ImportError:
    from itertools import izip_longest as zip_longest


class ResolutionError(Exception):
    def __init__(self, message, position):
        Exception.__init__(self, message)
        self.position = position


def resolve(name, template_stack):
    def get_template():
        try:
            return template_stack.pop(0)
        except IndexError:
            raise ResolutionError('missing template specifier for template %s' % '::'.join(name.name), name.position)
    for object, arguments, template in name.targets[:-1]:
        if arguments:
            second_template = get_template()
            second_template.bind_arguments(template, arguments)
    object, arguments, template = name.targets[-1]
    if arguments and template_stack:
        second_template = get_template()
        second_template.bind_arguments(template, arguments)
    elif isinstance(object, Template):
        second_template = get_template()
        second_template.bind_arguments(template, second_template.parameters)
    elif template_stack:
        if object:
            object_type = object.get_token_type().split('_')[0].lower()
            raise ResolutionError('extra template specifier for %s %s' % (object_type, '::'.join(name.name)), name.position)
        template = get_template()
        template.create_binding()



class DependentName(CppObject):
    def __init__(self, parent, position, name):
        CppObject.__init__(self, parent, position)
        self.name = name
        self.resolved_to = None

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name, is_current_scope):
        return None

    def _instantiate(self, parent, template_arguments):
        if self.name.targets[0][0]:
            value = self.name.targets[0][0].instantiate(parent, template_arguments)
        else:
            value = (parent.lookup_by_name(self.name.name[0])[1]
                  or self.parser.lexer.lookup_by_name(self.name.name[0])[1])
        if not value:
            return DependentTypeName(parent, self.position, self.name.instantiate(parent, template_arguments))
        prev_name = self.name.name[0]
        for name, target in zip(self.name.name[1:], self.name.targets[1:]):
            try:
                value = value.find(name, True).instantiate(parent, template_arguments)
            except AttributeError:
                self.parser.lexer._error("%s cannot be used prior to '::' because it has no members" % name.to_string(),
                                         self.name.position)
            if not value:
                self._error('%s does not refer to a member of %s' % (name, prev_name))
            if target[1]:
                if not isinstance(value, Template):
                    self._error('%s does not refer to a template member of %s' % (name, prev_name))
                try:
                    value = value.find_instance(parent,
                                                [t.instantiate(parent, template_arguments) for t in target[1]],
                                                self.name.position)
                except Template.InstanciationError as e:
                    self.parser.lexer._error(e.msg, e.position)
                    if e.error:
                        self.parser.lexer._note(e.error.message, e.error.position)
                    self.parser.lexer._note('template %s declared here' % value.name, value.position)
                else:
                    if not value:
                        return DependentTypeName(parent, self.position, self.name.instantiate(parent, template_arguments))
            prev_name = name
        return value


class TemplateParameterValue:
    def __init__(self, owner, index):
        self.owner = owner
        self.index = index

    def instantiate(self, template_arguments):
        value = template_arguments[1][self.index][1]
        print(self.owner.name, '->', value.__class__.__name__, value.to_string())
        return value


class TemplateParameter(CppObject):
    def __init__(self, parent, position, name):
        CppObject.__init__(self, parent, position)
        self.name = name
        self.bound_value = None

    def bind(self, value):
        if self.bound_value:
            raise Exception('cannot bind twice a template parameter')
        self.bound_value = value

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def instantiate(self, parent, template_arguments):
        if self.bound_value.owner.root_template == template_arguments[0].root_template:
            return self.bound_value.instantiate(template_arguments)
        else:
            for template, instance in template_arguments[2]:
                if template.root_template == self.bound_value.owner.root_template:
                    return self._instantiate(instance.root_template, parent, template_arguments)
            return self


class TemplateParameterConstant(TemplateParameter):
    def __init__(self, parent, position, type, name, default_value):
        TemplateParameter.__init__(self, parent, position, name)
        self.type = type
        self.default_value = default_value

    def get_token_type(self):
        return 'VARIABLE_ID'

    def is_valid(self, other):
        from .types import ConversionError
        if not isinstance(other, TemplateParameterConstant):
            raise ConversionError('cannot convert from %s to %s constant' % (other.__class__.__name__.lower(),
                                                                             self.type.name()),
                                         self.position)
        self.type.try_conversion(other.type)

    def to_string(self):
        return '/*%s*/ %s/*%s*/' % (self.type.type_name(), self.name,
                                    self.default_value and self.default_value.to_string() or '')

    def _instantiate(self, new_template, parent, template_arguments):
        default_value = self.default_value and self.default_value.instantiate(parent, template_arguments)
        result = TemplateParameterConstant(parent, self.position,
                                           self.type.instantiate(parent, template_arguments),
                                           self.name,
                                           default_value)
        result.bind(TemplateParameterValue(new_template, self.bound_value.index))
        return result

    def wrap_template_parameter(self):
        from .values import DependentValueName
        name = Name(self.parser.lexer, (self.name,), self.position, self, dependent=True)
        return DependentValueName(self.parent, self.position, name)


class TemplateParameterTypename(TemplateParameter):
    def __init__(self, parent, position, name, default_value):
        TemplateParameter.__init__(self, parent, position, name)
        self.default_value = default_value

    def get_token_type(self):
        return 'TYPENAME_ID'

    def type_name(self):
        return self.name

    def find(self, name, is_current_scope):
        return None

    def signature(self):
        return 'typename{%s}' % (self.name)

    def try_conversion(self, other, qualifier_importance = False):
        from .types import ConversionError
        if not isinstance(other, TemplateParameterTypename):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        if self.bound_value != other.bound_value:
            raise ConversionError("can't convert from %s to %s" % (self.name, other.name),
                                  self.position)

    def to_string(self):
        return 'typename %s%s' % (self.name,
                                  self.default_value and self.default_value.to_string() or '')

    def _instantiate(self, new_template, parent, template_arguments):
        default_value = self.default_value and self.default_value.instantiate(parent, template_arguments)
        result = TemplateParameterTypename(parent, self.position, self.name,
                                           default_value)
        result.bind(TemplateParameterValue(new_template, self.bound_value.index))
        return result

    def __str__(self):
        return '%s[from %s %s]' % (self.name, self.parent.name, id(self.parent))

    def wrap_template_parameter(self):
        from .types import DependentTypeName
        name = Name(self.parser.lexer, (self.name,), self.position, self, dependent=True)
        return DependentTypeName(self.parent, self.position, name)


class Template(CppObject):
    class InstanciationError(Exception):
        def __init__(self, msg, position, error):
            self.msg = msg
            self.position = position
            self.error = error

    def __init__(self, parent, position):
        CppObject.__init__(self, parent, position)
        self.parameters = []
        self.specializations = []
        self.created_instances = []
        self.name = None
        self.root_template = self
        self.specialization = False

    def get_token_type(self):
        return 'TEMPLATE_' + self.specializations[0][1].get_token_type()

    def find_nonrecursive(self, name):
        if self.name == name and not self.specialization:
            return self

    def add_template_parameter(self, parameter):
        self.parameters.append(parameter)

    def find(self, name, is_current_scope):
        if name == self.name:
            return self.specializations[0][1]
        for p in self.parameters:
            if p.name == name:
                return p
            
    def find_instance(self, parent, arguments, position):
        template_decl = True
        template_arguments = []
        if len(arguments) > len(self.parameters):
            parameters = ', '.join([p.to_string() for p in self.parameters])
            self.parser.lexer._error('wrong number of template arguments (%d, should be %d)' % (len(arguments), len(self.parameters)),
                                     position)
            self.parser.lexer._note('provided for template %s<%s>' % (self.name, parameters), self.position)
            return None

        for parameter, argument in zip_longest(self.parameters, arguments):
            if not argument and not a.default_value:
                parameters = ', '.join([p.to_string() for p in self.parameters])
                self.parser.lexer._error('wrong number of template arguments (%d, should be %d)' % (len(arguments), len(self.parameters)),
                                         position)
                self.parser.lexer._note('provided for template %s<%s>' % (self.name, parameters), self.position)
                return None
            template_arguments.append((parameter.name, argument or parameter.default_value))

        #for specialization_arguments, specialization in self.specializations:
        #    for index, arg in enumerate(arguments):
        #        template_arg =
        if template_decl:
            return self.specializations[0][1]
        else:
            return self.create_instance(specialization, parent, template_arguments, position)

    def create_instance(self, specialization, parent, arguments, position):
        instance = specialization.instantiate(parent, (self, template_arguments, []))
        self.created_instances.append((arguments, instance))
        instance_name = '%s< %s >' % (self.name, (', ').join([a.to_string() for a in arguments]))
        print(u'\u256d%s\u256e' % (u'\u2500'*(23+len(instance_name))))
        print(u'\u2502 creating instance of %s \u2502' % instance_name)
        print(u'\u251c%s\u256f' % (u'\u2500'*(23+len(instance_name))))
        instance._debug_dump(u'\u2502 ')
        print(u'\u2570%s\u2500' % (u'\u2500'*(23+len(instance_name))))
        return instance

    def _instantiate(self, parent, template_arguments):
        result = Template(parent, self.position)
        result.name = self.name
        new_arguments = (template_arguments[0],
                         template_arguments[1],
                         template_arguments[2] + [(self, result)])
        for p in self.parameters:
            result.add_template_parameter(p.instantiate(result, new_arguments))
        for params, s in self.specializations:
            instance_params = (p.instantiate(result, new_arguments) for p in params)
            instance_specialization = s.instantiate(result, new_arguments)
            result.specialize(tuple(instance_params), instance_specialization)
        for (arguments, instance) in self.created_instances:
            new_arguments = [a.instantiate(result, template_arguments) for a in arguments]
            result.created_instances.append((new_arguments,
                                             instance.instantiate(parent, template_arguments)))
        return result

    def add(self, specialization):
        if not self.name:
            self.name = specialization.name
        self.specializations.append(([p.wrap_template_parameter() for p in self.parameters],
                                     specialization))

    def specialize(self, parameters, template_specialization):
        if not self.name:
            self.name = template_specialization.name
        self.specializations.append((parameters, template_specialization))

    def create_template_arguments(self, arguments, position):
        template_arguments = []
        from .types import ConversionError
        for parameter, argument in zip_longest(self.parameters, arguments):
            if not argument:
                raise Template.InstanciationError('too many template parameters', position, None)
            if not parameter:
                if not agument.default_value:
                    raise Template.InstanciationError('too few template parameters', position, None)
                else:
                    argument = parameter.default_value
            try:
                argument.is_valid(parameter)
            except AttributeError:
                raise Template.InstanciationError('Invalid value for template parameter %s' % parameter.name,
                                                  argument.position,
                                                  None)
            except ConversionError as e:
                if parameter.name:
                    raise Template.InstanciationError('Invalid value for template parameter %s' % parameter.name,
                                                      argument.position,
                                                      e)
                else:
                    raise Template.InstanciationError('Invalid value for template parameter',
                                                      argument.position,
                                                      e)
            template_arguments.append((parameter.name, argument))
        return self.specializations[0][1], template_arguments

    def write_to(self, writer):
        for arguments, instance  in self.created_instances:
            instance.write_to(writer)

    def create_binding(self):
        for index, parameter in enumerate(self.parameters):
            parameter.bind(TemplateParameterValue(self, index))

    def bind_arguments(self, original_template, argument_list):
        self.root_template = original_template
        for index, parameter in enumerate(self.parameters):
            parameter.bind(TemplateParameterValue(original_template, index))

    def _debug_dump(self, indent):
        parameters = ', '.join([p.to_string() for p in self.parameters])
        if self.specializations:
            print(indent + 'template< %s >' % parameters)
            print(indent + '/* %s->%s */' % (id(self), id(self.root_template)))
            self.specializations[0][1]._debug_dump(indent)

            #TODO: specializations
            for arguments, instance in self.created_instances:
                print(indent + '`-+- instance <%s>' % ', '.join([a.to_string() for a in arguments]))
                instance._debug_dump(indent + '  |  ')
        else:
            print(indent + '/* template< %s > */' % parameters)
            print(indent + '/* %s->%s */' % (id(self), id(self.root_template)))
            print(indent + '/* %s definition */' % (self.root_template.specializations[0][1].name))

    def type_name(self):
        return self.name
