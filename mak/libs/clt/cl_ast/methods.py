from .cppobject import CppObject
from .scope import Scope
from . import types


class Parameter(CppObject):
    def __init__(self, lexer, position, name, type, default_value):
        CppObject.__init__(self, lexer, position, name)
        self.type = type
        self.default_value = default_value

    def get_token_type(self):
        return 'VARIABLE_ID'

    def _create_template_instance(self, template, arguments, position):
        return Parameter(self.lexer, self.position, self.name,
                         self.type.create_template_instance(template, arguments, position),
                         self.default_value and self.default_value.create_template_instance(template, arguments, position))


class OverloadScope(Scope):
    pass

class Overload(CppObject):
    def __init__(self, lexer, position, parameters, return_type, attributes):
        CppObject.__init__(self, lexer, position)
        self.parameters = parameters[:]
        self.return_type = return_type
        self.attributes = attributes

    def match(self, parameters, return_type, attributes):
        if len(parameters) > len(self.parameters):
            return False
        for p1, p2 in zip(self.parameters, parameters):
            try:
                p1.type.distance(p2.type, types.CAST_NONE)
            except types.CastError:
                return False
        for p in self.parameters[len(parameters):]:
            if not p.default_value:
                return False
        return True
    
    def _create_template_instance(self, template, arguments, position):
        return Overload(self.lexer, self.position,
                        [p.create_template_instance(template, arguments, position) for p in self.parameters],
                        self.return_type.create_template_instance(template, arguments, position),
                        self.attributes)


class Method(CppObject):
    index = 1
    def __init__(self, lexer, position, name):
        CppObject.__init__(self, lexer, position, name)
        self.id = Method.index
        Method.index += 1
        self.overloads = []

    def find_overload(self, parameters, return_type, attributes):
        for o in self.overloads:
            if o.match(parameters, return_type, attributes):
                return o
        return None

    def create_overload(self, position, parameters, return_type, attributes):
        o = Overload(self.lexer, position, parameters, return_type, attributes)
        self.overloads.append(o)
        return o

    def get_token_type(self):
        return 'METHOD_ID'

    def _create_template_instance(self, template, arguments, position):
        return Method(self.lexer, self.position, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        for o in self.overloads:
            result.overloads.append(o.create_template_instance(template, arguments, position))
        return result

    def write_to(self, writer):
        for i, o in enumerate(self.overloads):
            with writer.create_method(o.position, self.id, self.name, i) as method:
                pass