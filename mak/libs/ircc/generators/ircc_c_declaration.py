from .ircc_c_types import IrccCTypes
from .. import IrccType
from be_typing import TYPE_CHECKING


class IrccCDeclaration(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)

    def begin_module(self):
        # type: () -> bool
        self._out_file.write(
            '/* generated code; do not edit */\n'
            '#include <stdint.h>\n'
            'typedef int8_t  i8;\n'
            'typedef int16_t i16;\n'
            'typedef int32_t i32;\n'
            'typedef int64_t i64;\n'
            'typedef void* metadata;\n'
            '\n'
        )
        return True

    def header_specifier(self, name, value):
        # type: (str, str) -> None
        if name == 'source_filename':
            self._out_file.write('/* generated from %s */\n' % value)

    def end_module(self):
        # type: () -> None
        pass

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        if type._declaration[0]:
            type = IrccType([type._declaration[0], '', type._declaration[2], type._declaration[3]])
            self._out_file.write('/* %s */\ntypedef %s;\n\n' % (ir_name, type.format(['%s_' % name, '', name, ''])))
        else:
            self._out_file.write('/* %s */\ntypedef %s;\n\n' % (ir_name, type.format(['', '', name, ''])))

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str) -> bool
        self._out_file.write(
            '%s %s(%s)' % (
                return_type.format(['', '', '', '']
                                   ), name, ', '.join('%s' % t.format(['', '', n, '']) for t, n in parameters)
            )
        )
        return True

    def end_method(self):
        # type: () -> None
        self._out_file.write(';\n\n')


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType