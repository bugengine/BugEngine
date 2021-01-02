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

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str) -> bool
        return True

    def end_method(self):
        # type: () -> None
        pass


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from .ircc_type import IrccType