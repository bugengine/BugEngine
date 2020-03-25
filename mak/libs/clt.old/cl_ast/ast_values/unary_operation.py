from ..value import Value
from .constant import Constant


class UnaryOperation(Value):
    def __init__(self, lexer, position, operation, operand):
        # type: (ClLexer, Position, str, Value) -> None
        Value.__init__(self, lexer, position)
        self.operation = operation
        self.operand = operand

    def simplify_value(self):
        # type: () -> Value
        self.operand = self.operand.simplify_value()
        if isinstance(self.operand, Constant):
            return Constant(self.lexer, self.position, self.return_type(), self.evaluate(self.operand, self.operation))
        else:
            return self

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return self.operand.get_unresolved_parameters()

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> UnaryOperation
        return UnaryOperation(
            self.lexer, self.position, self.operation, self.operand
            and self.operand.create_template_instance(template, arguments, position)
        )

    def return_type(self):
        # type: () -> TypeRef
        return self.operand.return_type() #.get_unary_operation_type(self.operation)

    def __str__(self):
        # type: () -> str
        return '%s%s' % (self.operation, self.operand)

    @staticmethod
    def evaluate(operand, operator):
        # type: (Constant, str) -> Union[int, float]
        assert isinstance(operand, Constant)
        operations = {
            '+': lambda x: x,
            '-': lambda x: -x,
            '!': lambda x: not x,
            '~': lambda x: ~x,
        }
        result = operations[operator](operand.value)
        #print('simplifying: %s %s = %s' % (operator, operand.value, result))
        return result

    def write_to(self, namespace, writer):
        # type: (List[str], ClDocumentWriter) -> None
        pass


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import List, Union
    from ..typeref import TypeRef
    from ...cl_lexer import ClLexer
    from ...cl_codegen import ClDocumentWriter
    from ..position import Position
    from ..ast_templates import BaseTemplateParameter, Template
    from ..argument_list import ArgumentList