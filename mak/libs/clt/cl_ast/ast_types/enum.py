from ..value import Value
from ..type import Type, CastError
from ..typeref import TypeRef
from .builtin import BuiltIn
from ..scope import Scope
from ..ast_values.constant import Constant
from ..ast_values.binary_operation import BinaryOperation
from ..cppobject import CppObject


class EnumItem(Value):
    def __init__(self, lexer, position, name, value):
        # type: (ClLexer, Position, Optional[str], Optional[Value]) -> None
        Value.__init__(self, lexer, position, name)
        if not value:
            enum = self.parent
            assert isinstance(enum.scope, EnumScope)
            assert isinstance(enum, Enum)
            if len(enum.scope.items):
                last_item = enum.scope.items[-1][1]
                assert isinstance(last_item, EnumItem)
                value = BinaryOperation(
                    lexer, position, '+', Constant(lexer, position, lexer.base_types['u32'], 1), last_item.value
                )
            else:
                value = Constant(lexer, position, lexer.base_types['u32'], 0)
        self.value = value.simplify_value() # type: Value

    def get_token_type(self):
        # type: () -> str
        return 'VARIABLE_ID'

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> EnumItem
        return EnumItem(
            self.lexer, self.position, self.name, self.value.create_template_instance(template, arguments, position)
        )

    def return_type(self):
        # type: () -> TypeRef
        enum = self.parent
        assert isinstance(enum, Enum)
        return enum.type

    def debug_dump(self, indent=''):
        # type: (str) -> None
        print(
            '%s%s%s=%s [%s]' %
            (indent, self.__class__.__name__, self.name and (' %s' % self.name) or '', self.value, self.position)
        )

    def simplify_value(self):
        # type: () -> Value
        return self.value

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return self.value.get_unresolved_parameters()

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


class EnumScope(Scope):
    def __init__(self, owner, position):
        # type: (Enum, Position) -> None
        Scope.__init__(self, owner, position)


class Enum(Type):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    def __init__(self, lexer, position, name):
        # type: (ClLexer, Position, Optional[str]) -> None
        Type.__init__(self, lexer, position, name)
        self.struct_type = 'enum'
        self.type = TypeRef(lexer, position, self)
        self.type.qualifiers.add('const')

    def get_token_type(self):
        # type: () -> str
        return 'STRUCT_ID'

    def define(self, position):
        # type: (Position) -> None
        self.push_scope_recursive(position, EnumScope(self, position))

    def find(self, name):
        # type: (str) -> Optional[CppObject]
        return self.scope.find(name, self.position, self, False) or Type.find(self, name)

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass

    def _distance(self, other, cast_options, typeref, other_typeref):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if self == other:
            d = Type.Distance()
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif isinstance(other, BuiltIn):
            if cast_options.allowed_cast >= cast_options.CAST_IMPLICIT:
                d = Type.Distance(cast=1)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
            else:
                raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)
        elif isinstance(other, Enum):
            if cast_options.allowed_cast >= cast_options.CAST_STATIC:
                d = Type.Distance(cast=1)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
            else:
                raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant=-1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Enum
        return Enum(self.lexer, self.position, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        # type: (Enum, Template, ArgumentList, Position) -> None
        if self.scope:
            result.define(self.scope.position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception:
                result.pop_scope_recursive()
                raise
            else:
                result.pop_scope_recursive()


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import List, Optional
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..ast_templates import BaseTemplateParameter, Template
    from ..argument_list import ArgumentList
    from ..type import CastOptions
    from ..position import Position
