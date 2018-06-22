from .scope import Scope
from .types import Type, Pointer


class Parameter:
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


class Overload(Scope):
    def __init__(self, return_type, parameters, attributes, is_member, position):
        Scope.__init__(self, position)
        self.return_type = return_type
        self.parameters = parameters
        if is_member:
            if 'static' not in attributes:
                rtype = Type(is_member, is_member.position)
                if 'const' in attributes:
                    rtype.add_attribute('const')
                rtype = Type(Pointer(rtype, is_member.position), is_member.position)
                p = Parameter(rtype, 'this', None, [], is_member.position)
                self.parameters = [p] + self.parameters
        for p in self.parameters:
            self.add(p)
        self.attributes = attributes
        self.definition = None
        self.this = None
        self.signature = '|'.join((p.type.signature() for p in self.parameters))

    def define(self, definition):
        self.definition = definition


class Method(Scope):
    def __init__(self, method_name, position, owner):
        Scope.__init__(self, position)
        self.name = method_name
        self.overloads = []
        self.owner = owner

    def get_token_type(self):
        return 'METHOD_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find_overload(self, return_type, parameters, attributes, allow_creation, position):
        new_overload = Overload(return_type, parameters, attributes, self.owner, position)
        new_overload_static = Overload(return_type, parameters, attributes+['static'], self.owner, position)
        for overload in self.overloads:
            if overload.signature == new_overload.signature:
                return overload
            if not allow_creation and overload.signature == new_overload_static.signature:
                return overload
        if allow_creation:
            self.overloads.append(new_overload)
            return new_overload
        else:
            return None


class Body(Scope):
    pass
