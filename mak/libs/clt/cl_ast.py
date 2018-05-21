class Scope:
    def __init__(self, position):
        self.members = []
        self.position = position

    def find(self, name):
        for m in self.members:
            sub = m.find_nonrecursive(name)
            if sub:
                return sub

    def add(self, member):
        self.members.append(member)

    def instantiate(self, template_arguments):
        return self


class Root(Scope):
    def write_to(self, writer):
        writer.begin_document()
        writer.end_document()


class Namespace(Scope):
    def __init__(self, name, position):
        Scope.__init__(self, position)
        self.name = name

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def dump(self, indent=''):
        print('namespace %s\n{' % self.name)
        for m in self.members:
            m.dump(indent)
        print('}')

    def find_nonrecursive(self, name):
        if self.name == name:
            return self


class AnonymousNamespace(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        o = self.find(name)
        return o


class Typedef:
    def __init__(self, name, type, position):
        self.name = name
        self.type = type
        self.position = position

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name

    def find(self, name):
        return None

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return Typedef(self.name, self.type.instantiate(params), self.position)


class Typename:
    def __init__(self, name, default_value):
        self.name = name
        self.default_value = default_value

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)


class Template:
    def __init__(self):
        self.parameters = []
        self.specializations = []
        self.name = None

    def get_token_type(self):
        return 'TEMPLATE_' + self.specializations[0][1].get_token_type()

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def add_template_parameter(self, parameter):
        self.parameters.append(parameter)

    def find(self, name):
        for p in self.parameters:
            if p.name == name:
                return p

    def create_instance(self, arguments):
        template_arguments = {}
        return self.specializations[0][1].instantiate(template_arguments)

    def instantiate(self, template_arguments):
        result = Template()
        for p in self.parameters:
            result.add_template_parameter(p.instantiate(template_arguments))
        for params, s in self.specializations:
            result.add((p.instantiate(template_values) for p in params), s.instantiate(template_values))
        return result

    def add(self, specialization):
        if not self.name:
            self.name = specialization.name
        self.specializations.append(([], specialization))

    def specialize(self, parameters, template_specialization):
        if self.name == template_specialization.name:
            self.specializations.append((parameters, template_specialization))
            return True


class Struct:
    class Definition:
        def __init__(self, parent):
            self.parent = parent
            self.members = []

    def __init__(self, struct_type, name, position):
        self.name = name
        self.position = position
        self.struct_type = struct_type
        self.definition = None

    def add(self, member):
        self.definition.members.append(member)

    def find(self, name):
        if self.definition:
            for m in self.definition.members:
                sub = m.find_nonrecursive(name)
                if sub:
                    return sub

    def define(self, parent):
        self.definition = Struct.Definition(parent)

    def instantiate(self, template_arguments):
        return self

    def get_token_type(self):
        return 'STRUCT_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name


class Method(Scope):
    def __init__(self, method_name, return_type, attributes, position):
        Scope.__init__(self, position)
        self.name = method_name
        self.return_type = return_type
        self.attributes = attributes
        self.definition = None

    def define(self, definition):
        self.definition = definition

    def get_token_type(self):
        return 'METHOD_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self


class Builtin:
    def __init__(self, typename, position):
        self.typename = typename
        self.position = position

    def type_name(self):
        return self.typename

    def original_type(self):
        return self


class Type:
    def __init__(self, base_type):
        self.base_type = type
        self.modifier = []

    def add_modifier(self, modifier, position):
        self.modifier.append((modifier, position))

    def type_name(self):
        return ' '.join([self.base_type.type_name()] + self.modifier)

    def original_type(self):
        return self.base_type.original_type()


class Pointer:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '*'

    def original_type(self):
        return self.pointer_to.original_type()


class Reference:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '*'

    def original_type(self):
        return self.pointer_to.original_type()


class Array:
    def __init__(self, array_type, count, position):
        self.array_type = array_type
        self.count = count
        self.position = position

    def type_name(self):
        if self.count:
            return '%s[%s]' % (self.array_type.type_name(), self.count.write_expr())
        else:
            return '%s[]' % (self.array_type.type_name())

    def original_type(self):
        return self.array_type.original_type()


class Constant:
    def __init__(self, type, name, value, position):
        self.type = type
        self.name = name
        self.value = value
        self.position = position

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'


class Variable:
    def __init__(self, type, name, value, attributes, position):
        self.type = type
        self.name = name
        self.value = value
        self.attributes = attributes
        self.position = position

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'


class Specifier:
    def __init__(self, specifier, position):
        self.specifier = specifier
        self.position = position


class ForStatement(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def set_init(self, init):
        self.init = init
