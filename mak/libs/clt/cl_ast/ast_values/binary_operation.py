from ..value import Value
from .constant import Constant


class BinaryOperation(Value):
    def __init__(self, lexer, position, operation, left_operand, right_operand):
        # type: (ClLexer, Position, str, Value, Value) -> None
        Value.__init__(self, lexer, position)
        self.operation = operation
        self.left_operand = left_operand
        self.right_operand = right_operand

    def simplify_value(self):
        # type: () -> Value
        self.left_operand = self.left_operand.simplify_value()
        self.right_operand = self.right_operand.simplify_value()
        if isinstance(self.left_operand, Constant) and isinstance(self.right_operand, Constant):
            return Constant(
                self.lexer, self.position, self.return_type(),
                self.evaluate(self.left_operand, self.right_operand, self.operation)
            )
        else:
            return self

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return self.left_operand.get_unresolved_parameters() + self.right_operand.get_unresolved_parameters()

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> BinaryOperation
        return BinaryOperation(
            self.lexer, self.position, self.operation, self.left_operand
            and self.left_operand.create_template_instance(template, arguments, position), self.right_operand
            and self.right_operand.create_template_instance(template, arguments, position)
        )

    def return_type(self):
        # type: () -> TypeRef
        return self.left_operand.return_type().get_operation_type(self.right_operand.return_type(), self.operation)

    def __str__(self):
        # type: () -> str
        return '%s %s %s' % (self.left_operand, self.operation, self.right_operand)

    @staticmethod
    def evaluate(left_operand, right_operand, operator):
        # type: (Constant, Constant, str) -> Union[int, float]
        assert isinstance(left_operand, Constant)
        assert isinstance(right_operand, Constant)
        operations = {
            '+': lambda x, y: x + y,
            '-': lambda x, y: x - y,
            '*': lambda x, y: x * y,
            '/': lambda x, y: x / y,
            '<<': lambda x, y: x << y,
            '>>': lambda x, y: x >> y,
            '&': lambda x, y: x & y,
            '|': lambda x, y: x | y,
            '^': lambda x, y: x ^ y,
            '&&': lambda x, y: x and y,
            '||': lambda x, y: x or y,
        }
        v1 = left_operand.value
        v2 = right_operand.value
        result = operations[operator](v1, v2)
        #print('simplifying: %s %s %s = %s' % (v1, operator, v2, result))
        return result

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        pass


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import List, Union
    from ..typeref import TypeRef
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..position import Position
    from ..ast_templates import BaseTemplateParameter, Template
    from ..argument_list import ArgumentList
