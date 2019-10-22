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


class CodeBlock(Scope):
    def is_definition_scope(self):
        return False


class OverloadScope(CodeBlock):
    pass


class Overload(CppObject):
    def __init__(self, lexer, position, name, parameters, return_type, attributes):
        CppObject.__init__(self, lexer, position, name)
        self.parameters = parameters[:]
        self.return_type = return_type
        if return_type:
            assert not return_type.is_void()
        self.attributes = attributes

    def match(self, parameters, position, return_type, attributes):
        #attributes should be reflected in argument signature instread
        if self.attributes != attributes:
            return False
        if len(parameters) != len(self.parameters):
            return False
        for p1, p2 in zip(self.parameters, parameters):
            try:
                p1.type.distance(p2.type, types.CAST_NONE)
            except types.CastError:
                return False
        for p1, p2 in zip(self.parameters, parameters):
            if p1.default_value and p2.default_value:
                self.lexer.error('redefinition of default argument', p2.position)
                self.lexer.note('previous definition is here', p1.position)
        if self.return_type and return_type:
            try:
                return_type.distance(self.return_type, types.CAST_NONE)
            except types.CastError:
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.return_type.position)
        elif return_type:
            if not return_type.is_void():
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.position)
        elif self.return_type:
            self.lexer.error('functions that differ only by return type cannot be overloaded', position)
            self.lexer.note('previous declaration is here', self.return_type.position)
        return True

    def _create_template_instance(self, template, arguments, position):
        return Overload(self.lexer, self.position, self.name,
                        [p.create_template_instance(template, arguments, position) for p in self.parameters],
                        self.return_type and self.return_type.create_template_instance(template, arguments, position),
                        self.attributes)

    def debug_dump(self, name, indent):
        print('%s%s%s(%s);\n' % (indent, self.return_type and ('%s '%self.return_type) or '', name,
                                 ', '.join('%s%s'%(str(p.type), p.name and (' %s'%p.name) or '') for p in self.parameters)))


class Method(CppObject):
    index = 1
    def __init__(self, lexer, position, name):
        CppObject.__init__(self, lexer, position, name)
        self.id = Method.index
        Method.index += 1
        self.overloads = []

    def find_overload(self, parameters, position, return_type, attributes):
        for o in self.overloads:
            if o.match(parameters, position, return_type, attributes):
                return o
        return None

    def create_overload(self, parameters, position, return_type, attributes):
        if return_type and return_type.is_void():
            return_type = None
        o = Overload(self.lexer, position, self.name, parameters, return_type, attributes)
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

    def debug_dump(self, indent):
        for overload in self.overloads:
            overload.debug_dump(self.name, indent)


class SpecialMethod(Method):
    def __init__(self, lexer, position, name, owner):
        Method.__init__(self, lexer, position, name)
        self.owner = owner

    def get_token_type(self):
        return 'SPECIAL_METHOD_ID'
