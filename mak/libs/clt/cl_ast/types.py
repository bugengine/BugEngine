
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
        return self.type.find(name)

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return Typedef(self.name, self.type.instantiate(params), self.position)

    def signature(self):
        return self.type.signature()


class Typename:
    def __init__(self, name, default_value):
        self.name = name
        self.default_value = default_value

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name):
        return None

    def type_name(self):
        return self.name

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)

    def signature(self):
        return 'typename{%s}' % (self.name)


class DependentTypeName:
    def __init__(self, name):
        self.name = name
        self.resolved_to = None

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name):
        return None

    def type_name(self):
        return self.name

    def original_type(self):
        return self.resolved_to

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)

    def signature(self):
        return 'typename{%s}' % ('::'.join(self.name.name))


class Struct:
    id = 1
    class Definition:
        def __init__(self, parent):
            self.parent = parent
            self.members = []
            self.constructor = None
            self.destructor = None

    def __init__(self, struct_type, name, position):
        self.id += 1
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

    def signature(self):
        return '%s{%d}' % (self.name, self.id)


class Builtin:
    def __init__(self, typename, position):
        self.typename = typename
        self.position = position

    def type_name(self):
        return self.typename

    def original_type(self):
        return self

    def signature(self):
        return 'builtin{%s}' % self.typename


class Type:
    def __init__(self, base_type, position):
        self.base_type = base_type
        self.modifier = []
        self.position = position

    def add_modifier(self, modifier, position):
        self.modifier.append((modifier, position))

    def type_name(self):
        return ' '.join([self.base_type.type_name()] + self.modifier)

    def original_type(self):
        return self.base_type.original_type()

    def is_valid(self, type):
        return isinstance(type, Typename)

    def signature(self):
        return self.base_type.signature()


class Pointer:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '*'

    def original_type(self):
        return self.pointer_to.original_type()

    def signature(self):
        return '%s*' % (self.pointer_to.signature())


class Reference:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '&'

    def original_type(self):
        return self.pointer_to.original_type()

    def signature(self):
        return '%s&' % (self.pointer_to.signature())


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

    def signature(self):
        return '%s[%s]' % (self.array_type.signature(), count or '')

    def original_type(self):
        return self.array_type.original_type()
