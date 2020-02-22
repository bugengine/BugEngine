from ..value import Value


class Constant(Value):
    def __init__(self, lexer, position, type, value):
        # type: (ClLexer, Position, TypeRef, Union[int, float]) -> None
        Value.__init__(self, lexer, position)
        self.type = type
        self.value = value

    def simplify_value(self):
        # type: () -> Value
        return self

    def return_type(self):
        # type: () -> TypeRef
        return self.type

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> Constant
        return Constant(
            self.lexer, position, self.type.create_template_instance(template, arguments, position), self.value
        )

    def __eq__(self, other):
        # type: (object) -> bool
        if isinstance(other, Constant):
            return self.value == other.value
        return False

    def __str__(self):
        # type: () -> str
        return '(%s)%s' % (self.type, self.value)

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]) -> str
        return '#%s' % str(self.value)

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Tuple, Union
    from ..typeref import TypeRef
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..position import Position
    from ..ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from ..argument_list import ArgumentList
