from .cppobject import CppObject
from .types import CastError, CastOptions, Type


class Value(CppObject):
    def distance(self, other, cast_options):
        from .templates import TemplateValueParameter
        if isinstance(other, TemplateValueParameter):
            t = self.return_type()
            d = t.distance(other.type, CastOptions(CastOptions.CAST_IMPLICIT,
                                                   cast_options.template_parameter_matches,
                                                   cast_options.template_bindings,
                                                   cast_options.current_template))
            return Type.Distance(100000+d.distance[0],
                                 d.distance[1],
                                 d.distance[2],
                                 d.matches)
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
        return self

    def _create_template_instance(self, template, arguments, position):
        return VariableReference(self.lexer, self.position, self.object_name,
                                 self.target and self.target.create_template_instance(template, arguments, position))


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

    @staticmethod
    def evaluate(self, left_operand, right_operand, operator):
        assert isinstance(self.left_operand, Constant)
        assert isinstance(self.right_operand, Constant)
        v1 = self.left_operand.value
        v2 = self.right_operand.value
        result = eval('%s%s%s' % (v1, operator, v2))
        print('simplifying: %s %s %s = %s' % (v1, operator, v2, result))
        return result