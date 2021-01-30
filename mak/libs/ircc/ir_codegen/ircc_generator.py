from be_typing import TYPE_CHECKING
from abc import abstractmethod


class IrccGenerator:
    def __init__(self, file):
        # type: (TextIO) -> None
        self._out_file = file

    @abstractmethod
    def type_metadata(self):
        # type: () -> IrccType
        raise NotImplementedError

    @abstractmethod
    def type_void(self):
        # type: () -> IrccType
        raise NotImplementedError

    @abstractmethod
    def type_undef(self):
        # type: () -> IrccType
        raise NotImplementedError

    @abstractmethod
    def type_zero(self):
        # type: () -> IrccType
        raise NotImplementedError

    @abstractmethod
    def type_builtin(self, builtin):
        # type: (str) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def type_declared(self, name):
        # type: (str) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_array(self, type, size):
        # type: (IrccType, int) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_vector(self, type, size):
        # type: (IrccType, int) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_ptr(self, type):
        # type: (IrccType) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_struct(self, fields):
        # type: (List[Tuple[IrccType, str]]) -> IrccType
        struct_body = ' '.join('%s;' % type.format(['', '', name, '']) for type, name in fields)
        raise NotImplementedError

    @abstractmethod
    def make_const(self, type):
        # type: (IrccType) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_void(self):
        # type: () -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_zero(self):
        # type: () -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_nullptr(self, type):
        # type: (IrccType) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_undef(self, width):
        # type: (int) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_bool(self, type, value):
        # type: (IrccType, bool) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_constant_int(self, type, value):
        # type: (IrccType, int) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_aggregate_value(self, values):
        # type: (List[IrccExpression]) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_value(self, vector_type, values):
        # type: (IrccType, List[IrccExpression]) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_value_broadcast(self, vector_type, value):
        # type: (IrccType, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_array_value(self, values):
        # type: (List[IrccExpression]) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_cast(self, result_type, cast, value):
        # type: (IrccType, str, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_address(self, value):
        # type: (IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_dereference(self, expression):
        # type: (IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_access(self, value, field_name):
        # type: (IrccExpression, str) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_subscript(self, value, index):
        # type: (IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_variable_reference(self, name):
        # type: (str) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_integer_binary_op(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_float_binary_op(self, operation, type, left_operand, right_operand):
        # type: (str, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_integer_binary_op(
        self, operation, result_type, vector_type, left_operand, right_operand
    ):
        # type: (str, IrccType, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_float_binary_op(self, operation, result_type, vector_type, left_operand, right_operand):
        # type: (str, IrccType, IrccType, IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_value_load(self, address):
        # type: (IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_value_call(self, method_name, arguments):
        # type: (str, List[IrccExpression]) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_extract(self, element_type, vector, index):
        # type: (IrccType, IrccExpression, IrccExpression) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_extract_constant(self, vector, index):
        # type: (IrccExpression, int) -> IrccExpression
        raise NotImplementedError

    @abstractmethod
    def make_expression_vector_shuffle(self, result, source_size, v1, v2, mask):
        # type: (IrccType, int, IrccExpression, IrccExpression, List[int]) -> IrccExpression
        raise NotImplementedError

    def begin_module(self):
        # type: () -> bool
        return True

    def end_module(self):
        # type: () -> None
        pass

    def begin_headers(self):
        # type: () -> bool
        return True

    def header_specifier(self, name, value):
        # type: (str, str) -> None
        pass

    def end_headers(self):
        # type: () -> None
        pass

    def begin_declarations(self):
        # type: () -> bool
        return True

    def end_declarations(self):
        # type: () -> None
        pass

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        pass

    def begin_method(self, name, return_type, parameters, calling_convention, has_implementation):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        return True

    def end_method(self):
        # type: () -> None
        pass

    def declare_variable(self, name, type, value=None):
        # type: (str, IrccType, Optional[IrccExpression]) -> None
        raise NotImplementedError

    def declare_label(self, name):
        # type: (str) -> None
        raise NotImplementedError

    def begin_loop(self, label):
        # type: (str) -> None
        raise NotImplementedError

    def end_loop(self):
        # type: () -> None
        raise NotImplementedError

    def begin_if(self, condition):
        # type: (IrccExpression) -> None
        raise NotImplementedError

    def begin_if_not(self, condition):
        # type: (IrccExpression) -> None
        raise NotImplementedError

    def begin_else(self):
        # type: () -> None
        raise NotImplementedError

    def end_if(self):
        # type: () -> None
        raise NotImplementedError

    def instruction_return_value(self, return_value):
        # type: (IrccExpression) -> None
        raise NotImplementedError

    def instruction_break(self, label):
        # type: (str) -> None
        raise NotImplementedError

    def instruction_continue(self, label):
        # type: (str) -> None
        raise NotImplementedError

    def instruction_assign(self, destination, value):
        # type: (IrccExpression, IrccExpression) -> None
        raise NotImplementedError

    def instruction_vector_insert(self, element_type, vector, index, element):
        # type: (IrccType, IrccExpression, IrccExpression, IrccExpression) -> None
        raise NotImplementedError

    def instruction_vector_insert_constant(self, vector, index, element):
        # type: (IrccExpression, int, IrccExpression) -> None
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List, Optional, TextIO, Tuple
    from .ircc_type import IrccType
    from .ircc_expression import IrccExpression