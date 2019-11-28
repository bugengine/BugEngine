from .cppobject import CppObject


class Constant(CppObject):
    def __init__(self, lexer, position, type, value):
        CppObject.__init__(self, lexer, position)
        self.type = type
        self.value = value

    def get_unresolved_parameters(self):
        return []

    def _create_template_instance(self, template, arguments, position):
        return self

class BinaryOperation(CppObject):
    def __init__(self, lexer, position, operation, left_operand, right_operand):
        CppObject.__init__(self, lexer, position)
        self.operation = operation
        self.left_operand = left_operand
        self.right_operand = right_operand

    def get_unresolved_parameters(self):
        return self.left_operand.get_unresolved_parameters() + self.right_operand.get_unresolved_parameters()

    def _create_template_instance(self, template, arguments, position):
        return BinaryOperation(self.lexer, self.position, self.operation,
                               self.left_operand and self.left_operand.create_template_instance(template, arguments, position),
                               self.right_operand and self.right_operand.create_template_instance(template, arguments, position))