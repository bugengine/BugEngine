from .cppobject import CppObject
from .types import Type, TypeRef, BuiltIn, CastError
from .scope import Scope


class EnumItem(CppObject):
    def __init__(self, lexer, position, name, value):
        from .values import Constant, BinaryOperation
        CppObject.__init__(self, lexer, position, name)
        if not value:
            enum = self.parent
            assert isinstance(enum, Enum)
            if len(enum.scope.items):
                value = BinaryOperation(lexer, position, '+',
                                        Constant(lexer, position, lexer.base_types['u32'], 1),
                                        enum.scope.items[-1][1].value)
            else:
                value = Constant(lexer, position, lexer.base_types['u32'], 0)
        self.value = value.simplify()
    
    def get_token_type(self):
        return 'VARIABLE_ID'

    def _create_template_instance(self, template, arguments, position):
        return EnumItem(self.lexer, self.position, self.name,
                        self.value.create_template_instance(template, arguments, position))

    def debug_dump(self, indent=''):
        print('%s%s%s=%s [%s]' % (indent, self.__class__.__name__,
                                 self.name and (' %s'%self.name) or '',
                                 self.value,
                                 self.position))

    def simplify(self):
        return self.value

class EnumScope(Scope):
    def __init__(self, owner, position):
        Scope.__init__(self, owner, position)


class Enum(Type):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    def __init__(self, lexer, position, name):
        Type.__init__(self, lexer, position,name)
        self.struct_type = 'enum'
    
    def get_token_type(self):
        return 'STRUCT_ID'
    
    def define(self, position):
        self.push_scope_recursive(position, EnumScope(self, position))

    def find(self, name):
        return self.scope.find(name, None, None, False) or Type.find(self, name)

    def write_to(self, writer):
        pass

    def _distance(self, other, cast_options, typeref, other_typeref):
        if self == other:
            d = Type.Distance()
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif isinstance(other, BuiltIn):
            if cast_options.allowed_cast >= cast_options.CAST_IMPLICIT:
                d = Type.Distance(cast=1)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
            else:
                raise CastError('type %s is not compatible with %s' % (self, other), self.position)
        elif isinstance(other, Enum):
            if cast_options.allowed_cast >= cast_options.CAST_STATIC:
                d = Type.Distance(cast=1)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
            else:
                raise CastError('type %s is not compatible with %s' % (self, other), self.position)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def _create_template_instance(self, template, arguments, position):
        return Enum(self.lexer, self.position, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        if self.scope:
            result.define(self.scope.position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception:
                result.pop_scope_recursive()
                raise
            else:
                result.pop_scope_recursive()
        return result


