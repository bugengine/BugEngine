from ..type import Type, CastError


class BuiltIn(Type):
    def __init__(self, lexer, position, builtin):
        # type: (ClLexer, Position, str) -> None
        Type.__init__(self, lexer, position)
        self.builtin = builtin

    def __str__(self):
        # type: () -> str
        return self.builtin

    def _distance(self, other, cast_options, typeref, other_typeref):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(other, BuiltIn):
            if self.builtin != other.builtin:
                if cast_options.allowed_cast >= cast_options.CAST_IMPLICIT:
                    d = Type.Distance(cast=1)
                    return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
                else:
                    raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant=-1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)

    def pretty_name(self):
        # type: () -> str
        return "builtin type '%s'" % self.builtin

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> BuiltIn
        return self

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '~%s' % (self.builtin)


class Void(Type):
    def __init__(self, lexer, position):
        # type: (ClLexer, Position) -> None
        Type.__init__(self, lexer, position)

    def __str__(self):
        # type: () -> str
        return 'void'

    def is_void(self):
        # type: () -> bool
        return True

    def _distance(self, other, cast_options, typeref, other_typeref):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(other, Void):
            d = Type.Distance(cast=0)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant=-1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref, to_type=other_typeref)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Void
        return self

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '~void'

    def pretty_name(self):
        # type: () -> str
        return "builtin type 'void'"


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from ..argument_list import ArgumentList
    from ..typeref import TypeRef
    from ..type import CastOptions
    from ..position import Position