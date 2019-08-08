from .cppobject import CppObject
from .types import Type, BuiltIn, CastError, CAST_IMPLICIT, CAST_UNRELATED, CAST_STATIC
from .scope import Scope


class EnumItem(CppObject):
    def __init__(self, lexer, position, name, value):
        CppObject.__init__(self, lexer, position, name)
        self.value = value
    
    def get_token_type(self):
        return 'VARIABLE_ID'

    def _create_template_instance(self, template, arguments, position):
        return EnumItem(self.lexer, self.position, self.name,
                        self.value and self.value.create_template_instance(template, arguments, position))


class EnumScope(Scope):
    def __init__(self, owner, position):
        Scope.__init__(self, owner, position)


class Enum(Type):
    def __init__(self, lexer, position, name):
        Type.__init__(self, lexer, position,name)
        self.struct_type = 'enum'
    
    def get_token_type(self):
        return 'STRUCT_ID'
    
    def define(self, position):
        self.push_scope(position, EnumScope(self, position))

    def write_to(self, writer):
        pass

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if self == other:
            d = Type.Distance()
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif isinstance(other, BuiltIn):
            if allowed_cast >= CAST_IMPLICIT:
                d = Type.Distance(cast=1)
                return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
            else:
                raise CastError()
        elif isinstance(other, Enum):
            if allowed_cast >= CAST_STATIC:
                d = Type.Distance(cast=1)
                return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
            else:
                raise CastError()
        elif allowed_cast == CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return Enum(self.lexer, self.position, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        if self.scope:
            result.define(self.scope.position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception:
                self.lexer.pop_scope(result.scope)
                raise
            else:
                self.lexer.pop_scope(result.scope)
        return result


