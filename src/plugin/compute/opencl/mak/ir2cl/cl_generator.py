from ircc.generators import IrccCDeclaration, IrccCDefinition
from ircc import IrccType
from be_typing import TYPE_CHECKING


class ClDeclaration(IrccCDeclaration):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDeclaration.__init__(self, file)
        self._address_spaces = {0: '__private', 1: '__global', 2: '__constant', 3: '__local', 4: '#invalid'}

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        if address_space:
            return IrccType(['', self._address_spaces[address_space], ''], type)
        else:
            return type

    def begin_module(self):
        # type: () -> None
        self._out_file.write(
            '/* generated code; do not edit */\n'
            'typedef bool         i1;\n'
            'typedef signed char  i8;\n'
            'typedef signed short i16;\n'
            'typedef signed int   i32;\n'
            'typedef signed long  i64;\n'
            'typedef void* metadata;\n'
            ''
        )

    def end_module(self):
        # type: () -> None
        self._out_file.write('\n')

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, str, List[Tuple[str, str]], str) -> None
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        IrccCDeclaration.begin_method(self, name, return_type, parameters, calling_convention)


class ClDefinition(IrccCDefinition):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDefinition.__init__(self, file)
        self._address_spaces = {0: '', 1: '__global', 2: '__constant', 3: '__local', 4: '__todo'}

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        if address_space:
            return IrccType(['', self._address_spaces[address_space], ''], type)
        else:
            return type

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, str, List[Tuple[str, str]], str) -> None
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        IrccCDefinition.begin_method(self, name, return_type, parameters, calling_convention)


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType