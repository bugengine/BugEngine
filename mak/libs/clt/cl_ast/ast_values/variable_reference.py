from ..value import Value


class VariableReference(Value):
    def __init__(self, lexer, position, object_name, target):
        # type: (ClLexer, Position, Name, Value) -> None
        Value.__init__(self, lexer, position)
        self.object_name = object_name
        self.target = target

    def return_type(self):
        # type: () -> TypeRef
        return self.target.return_type()

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return self.target.get_unresolved_parameters()

    def simplify_value(self):
        # type: () -> Value
        return self.target.simplify_value()

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> VariableReference
        return VariableReference(
            self.lexer, self.position, self.object_name, self.target
            and self.target.create_template_instance(template, arguments, position)
        )

    def __str__(self):
        # type: () -> str
        return str(self.object_name)

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ..typeref import TypeRef
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..position import Position
    from ..ast_templates import BaseTemplateParameter, Template
    from ..argument_list import ArgumentList
    from ..name import Name
    from ..variable import Variable