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

class Builtin:
    def __init__(self, typename):
        self.typename = typename

    def type_name(self):
        return self.typename

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

