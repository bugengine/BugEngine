from .ircc_c_expressions import IrccCExpressions
from .. import IrccType
from be_typing import TYPE_CHECKING


class IrccCDeclaration(IrccCExpressions):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCExpressions.__init__(self, file)

    def begin_module(self):
        # type: () -> bool
        self._out_file.write(
            '/* generated code; do not edit */\n'
            '#include <stdint.h>\n'
            'typedef int8_t  i8;\n'
            'typedef int16_t i16;\n'
            'typedef int32_t i32;\n'
            'typedef int64_t i64;\n'
            'typedef uint8_t  i8;\n'
            'typedef uint16_t i16;\n'
            'typedef uint32_t i32;\n'
            'typedef uint64_t i64;\n'
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

    def declare_variable(self, name, type, value=None):
        # type: (str, IrccType, Optional[IrccExpression]) -> None
        pass

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        if type._declaration[0]:
            type = IrccType([type._declaration[0], '', type._declaration[2], type._declaration[3]])
            self._out_file.write('/* %s */\ntypedef %s;\n\n' % (ir_name, type.format(['%s_' % name, '', name, ''])))
        else:
            self._out_file.write('/* %s */\ntypedef %s;\n\n' % (ir_name, type.format(['', '', name, ''])))

    def begin_method(self, name, return_type, parameters, calling_convention, has_body):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        self._out_file.write(
            '%s %s(%s);\n\n' % (
                return_type.format(['', '', '', '']
                                   ), name, ', '.join('%s' % t.format(['', '', n, '']) for t, n in parameters)
            )
        )
        return False

    def end_method(self):
        # type: () -> None
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List, Optional, TextIO, Tuple
    from ircc import IrccType, IrccExpression