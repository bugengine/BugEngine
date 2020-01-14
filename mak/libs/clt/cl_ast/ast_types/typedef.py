from ..type import Type
from ..scope import Scope
from ..error import CppError


class TypeDef(Type):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, CppObject, bool) -> Optional[CppObject]
            if is_current_scope:
                assert isinstance(self.owner, TypeDef)
                assert self.owner.type.type is not None
                scope = self.owner.type.type.scope
                try:
                    return scope.find(name, position, source_context, is_current_scope)
                except CppError as e:
                    raise e.__class__(
                        "with '%s' defined as %s" % (self.owner.name, self.owner.type.type.pretty_name()),
                        self.position, e
                    )
            else:
                return None

    def __init__(self, lexer, position, name, type):
        # type: (ClLexer, Position, Optional[str], TypeRef) -> None
        Type.__init__(self, lexer, position, name)
        self.type = type

    def get_type(self):
        # type: () -> Type
        return self.type.get_type()

    def __str__(self):
        # type: () -> str
        return str(self.type)

    def get_token_type(self):
        # type: () -> str
        return 'TYPENAME_ID'

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        try:
            return self.type.distance(typeref_to, cast_options)
        except CppError as e:
            assert self.type.type is not None
            raise e.__class__("with '%s' defined as %s" % (self.name, self.type.type.pretty_name()), self.position, e)

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return self.type.get_unresolved_parameters()

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> TypeDef
        return TypeDef(
            self.lexer, self.position, self.name, self.type.create_template_instance(template, arguments, position)
        )

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..ast_templates import BaseTemplateParameter, Template
    from ..cppobject import CppObject
    from ..argument_list import ArgumentList
    from ..position import Position
    from ..typeref import TypeRef
    from ..type import CastOptions
