from .scope import Scope
from .types import Type, Pointer
from .expressions import Specifier


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
    def __init__(self, owner, return_type, parameters, attributes, is_member, position):
        Scope.__init__(self, position)
        self.owner = owner
        self.return_type = return_type
        self.parameters = parameters
        if is_member:
            if 'static' not in [a.specifier for a in attributes]:
                rtype = Type(is_member, is_member.position)
                for a in attributes:
                    if a.specifier == 'const':
                        rtype.add_modifier('const', a.position)
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
    id = 1
    def __init__(self, method_name, position, owner):
        Scope.__init__(self, position)
        self.id = Method.id
        Method.id += 1
        self.name = method_name
        self.overloads = []
        self.owner = owner

    def get_token_type(self):
        return 'METHOD_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find_overload(self, return_type, parameters, attributes, allow_creation, position):
        new_overload = Overload(self, return_type, parameters, attributes, self.owner, position)
        new_overload_static = Overload(self, return_type, parameters,
                                       attributes+[Specifier('static', position)],
                                       self.owner, position)
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

    def write_to(self, writer):
        for overload_id, overload in enumerate(self.overloads):
            with writer.create_method(self.position, self.id, self.name, overload_id) as m:
                pass


class SpecialMethod(Method):
    def get_token_type(self):
        return 'SPECIAL_METHOD_ID'

class Body(Scope):
    pass
