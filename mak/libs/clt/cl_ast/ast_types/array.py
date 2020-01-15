from ..type import Type, CastError


class Array(Type):
    def __init__(self, lexer, position, type, size):
        # type: (ClLexer, Position, TypeRef, Optional[Value]) -> None
        Type.__init__(self, lexer, position)
        self.type = type
        self.size = size

    def __str__(self):
        # type: () -> str
        return '%s[%s]' % (str(self.type), self.size or '')

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(cast_to, Array):
            allowed_cast = cast_options.allowed_cast
            if cast_options.allowed_cast in (cast_options.CAST_STATIC, cast_options.CAST_UPCAST):
                cast_options.allowed_cast = cast_options.CAST_UPCAST
            elif cast_options.allowed_cast in (cast_options.CAST_REINTERPRET, cast_options.CAST_UNRELATED):
                cast_options.allowed_cast = cast_options.CAST_UNRELATED
            d = self.type.distance(cast_to.type, cast_options).refine()
            return d.match_attributes(allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Array
        return Array(
            self.lexer, self.position, self.type.create_template_instance(template, arguments, self.position), self.size
            and self.size.create_template_instance(template, arguments, self.position)
        )

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        return '[]' + self.type.signature(template_bindings)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Optional, Tuple
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..ast_templates import BaseTemplateParameter, Template
    from ..argument_list import ArgumentList
    from ..position import Position
    from ..type import CastOptions
    from ..typeref import TypeRef
    from ..value import Value