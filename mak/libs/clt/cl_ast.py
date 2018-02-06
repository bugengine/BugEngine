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

    def dump(self, indent=''):
        for m in self.members:
            m.dump(indent)


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

    def dump(self, indent=''):
        print('namespace\n{')
        for m in self.members:
            m.dump(indent)
        print('}')

    def find_nonrecursive(self, name):
        o = self.find(name)
        return o


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

    def get_token_type(self):
        return 'STRUCT_ID'

    def dump(self, indent=''):
        print('%s%s %s\n{' % (indent, self.struct_type, self.name))
        for m in self.definition.members:
            m.dump(indent+' ')
        print('};')

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name


class Method:
    def __init__(self, method_name, return_type):
        self.name = method_name
        self.return_type = return_type
        self.definition = None

    def define(self, definition):
        self.definition = definition

    def get_token_type(self):
        return 'METHOD_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self


class TypeRef:
    def __init__(self, struct_ref):
        self.struct_ref = struct_ref
        self.position = self.struct_ref.position

    def type_name(self):
        return "%s %s" % (self.struct_ref.struct_type, self.struct_ref.name)

    def original_type(self):
        return self


class Builtin:
    def __init__(self, typename, position):
        self.typename = typename
        self.position = position

    def type_name(self):
        return self.typename

    def original_type(self):
        return self


class Pointer:
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

    def dump(self, indent=''):
        print('%s%s%s %s%s;' % (indent, ' '.join(self.attributes) + (self.attributes and ' ' or ''),
                                self.type.type_name(), self.name, self.value and (' = %s' % self.value.str()) or ''))


class Specifier:
    def __init__(self, specifier, position):
        self.specifier = specifier
        self.position = position
