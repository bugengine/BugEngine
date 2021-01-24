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
    def make_value_void(self):
        # type: () -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_zero(self):
        # type: () -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_nullptr(self, type):
        # type: (IrccType) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_undef(self, width):
        # type: (int) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_bool(self, type, value):
        # type: (IrccType, bool) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_int(self, type, value):
        # type: (IrccType, int) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_aggregate(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_vector(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_array(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_cast(self, result_type, cast, value):
        # type: (IrccType, str, IrccValue) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_addressof(self, value):
        # type: (IrccValue) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_access(self, value, field_name):
        # type: (IrccValue, str) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_index(self, value, index):
        # type: (IrccValue, IrccValue) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_reference(self, name):
        # type: (str) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_binary_op(self, operation, left_operand, right_operand):
        # type: (str, IrccValue, IrccValue) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_load(self, address):
        # type: (IrccValue) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def make_value_call(self, method_name, arguments):
        # type: (str, List[IrccValue]) -> IrccValue
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

    def declare_variable(self, name, type):
        # type: (str, IrccType) -> None
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
        # type: (IrccValue) -> None
        raise NotImplementedError

    def begin_if_not(self, condition):
        # type: (IrccValue) -> None
        raise NotImplementedError

    def begin_else(self):
        # type: () -> None
        raise NotImplementedError

    def end_if(self):
        # type: () -> None
        raise NotImplementedError

    def instruction_return_value(self, return_value):
        # type: (IrccValue) -> None
        raise NotImplementedError

    def instruction_break(self, label):
        # type: (str) -> None
        raise NotImplementedError

    def instruction_continue(self, label):
        # type: (str) -> None
        raise NotImplementedError

    def instruction_assign(self, name, value):
        # type: (str, IrccValue) -> None
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from .ircc_type import IrccType
    from .ircc_value import IrccValue