from ircc.generators import IrccCDeclaration, IrccCDefinition
from ircc import IrccType
from be_typing import TYPE_CHECKING


class ClDeclaration(IrccCDeclaration):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDeclaration.__init__(self, file)
        self._address_spaces = {0: '__private', 1: '__global', 2: '__constant', 3: '__local', 4: '__todo'}

    def type_metadata(self):
        # type: () -> IrccType
        return IrccType(['', '', 'meta', ''])

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return IrccType(['', self._address_spaces[address_space], ''], type)

    def begin_module(self):
        # type: () -> bool
        self._out_file.write(
            '/* generated code; do not edit */\n'
            'typedef bool         i1;\n'
            'typedef signed char  i8;\n'
            'typedef signed short i16;\n'
            'typedef signed int   i32;\n'
            'typedef signed long  i64;\n'
            'typedef void* meta;\n'
            '\n'
        )
        return True

    def end_module(self):
        # type: () -> None
        self._out_file.write('\n')

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str) -> bool
        if name.startswith('llvm.'):
            # intrinsic
            return False
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        return IrccCDeclaration.begin_method(self, name, return_type, parameters, calling_convention)


class ClDefinition(IrccCDefinition):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCDefinition.__init__(self, file)
        self._address_spaces = {0: '__private', 1: '__global', 2: '__constant', 3: '__local', 4: '__todo'}

    def type_metadata(self):
        # type: () -> IrccType
        return IrccType(['', '', 'meta', ''])

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return IrccType(['', self._address_spaces[address_space], ''], type)

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str) -> bool
        if name.startswith('llvm.'):
            # intrinsic
            return False
        if calling_convention == 'spir_kernel_flat':
            self._out_file.write('__kernel\n')
        return IrccCDefinition.begin_method(self, name, return_type, parameters, calling_convention)


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType