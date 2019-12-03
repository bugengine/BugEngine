from .cppobject import CppObject
from .types import CastError, CastOptions, Type


class Value(CppObject):
    def distance(self, other, cast_options):
        from .templates import TemplateValueParameter
        if isinstance(other, TemplateValueParameter):
            return other.template_parameter_match(self, cast_options)
        elif isinstance(other, Value):
            if self == other:
                return Type.Distance()
            else:
                raise CastError('error', self.position)
        else:
            raise CastError('error', self.position)


class Constant(Value):
    def __init__(self, lexer, position, type, value):
        Value.__init__(self, lexer, position)
        self.type = type
        self.value = value
    
    def return_type(self):
        return self.type

    def get_unresolved_parameters(self):
        return []

    def _create_template_instance(self, template, arguments, position):
        return self
    
    def __eq__(self, other):
        if isinstance(other, Constant):
            return self.value == other.value
        return False

    def __str__(self):
        return '(%s)%s' % (self.type, self.value)

class VariableReference(Value):
    def __init__(self, lexer, position, object_name, target):
        Value.__init__(self, lexer, position)
        self.object_name = object_name
        self.target = target

    def return_type(self):
        return self.target.type

    def get_unresolved_parameters(self):
        return self.target.get_unresolved_parameters()

    def simplify(self):
        if isinstance(self.target, Constant):
            return self.target
        return self

    def _create_template_instance(self, template, arguments, position):
        return VariableReference(self.lexer, self.position, self.object_name,
                                 self.target and self.target.create_template_instance(template, arguments, position))

    def __str__(self):
        return str(self.object_name)

class BinaryOperation(Value):
    def __init__(self, lexer, position, operation, left_operand, right_operand):
        Value.__init__(self, lexer, position)
        self.operation = operation
        self.left_operand = left_operand
        self.right_operand = right_operand

    def simplify(self):
        self.left_operand = self.left_operand.simplify()
        self.right_operand = self.right_operand.simplify()
        if isinstance(self.left_operand, Constant) and isinstance(self.right_operand, Constant):
            return Constant(self.lexer, self.position, self.return_type(), self.evaluate(self.left_operand, self.right_operand, self.operation))
        else:
            return self

    def get_unresolved_parameters(self):
        return self.left_operand.get_unresolved_parameters() + self.right_operand.get_unresolved_parameters()

    def _create_template_instance(self, template, arguments, position):
        return BinaryOperation(self.lexer, self.position, self.operation,
                               self.left_operand and self.left_operand.create_template_instance(template, arguments, position),
                               self.right_operand and self.right_operand.create_template_instance(template, arguments, position))
    
    def return_type(self):
        return self.left_operand.return_type().get_operation_type(self.right_operand.return_type(), self.operation)

    def __str__(self):
        return '%s %s %s' % (self.left_operand, self.operation, self.right_operand)

    @staticmethod
    def evaluate(left_operand, right_operand, operator):
        assert isinstance(left_operand, Constant)
        assert isinstance(right_operand, Constant)
        v1 = left_operand.value
        v2 = right_operand.value
        result = eval('%s%s%s' % (v1, operator, v2))
        #print('simplifying: %s %s %s = %s' % (v1, operator, v2, result))
        return result