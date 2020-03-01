from ..type import Type, CastError


class Reference(Type):
    def __init__(self, lexer, position, type):
        # type: (ClLexer, Position, TypeRef) -> None
        Type.__init__(self, lexer, position)
        self.type = type

    def __str__(self):
        # type: () -> str
        return str(self.type) + '&'

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(cast_to, Reference):
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
            raise CastError(self.lexer.logger.C0300, self.position, from_type=typeref_from, to_type=typeref_to)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Reference
        return Reference(
            self.lexer, self.position, self.type.create_template_instance(template, arguments, self.position)
        )

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '&' + self.type.signature(template_bindings)

    def transform(self, writer):
        # type: (ClTypeWriter) -> ClType
        return writer.reference(self.type.transform(writer))


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Tuple
    from ...cl_lexer import ClLexer
    from ...cl_codegen import ClDocumentWriter, ClTypeWriter, ClType
    from ..ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from ..argument_list import ArgumentList
    from ..position import Position
    from ..typeref import TypeRef
    from ..type import CastOptions
