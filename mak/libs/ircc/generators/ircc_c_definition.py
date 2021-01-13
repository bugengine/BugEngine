from .ircc_c_types import IrccCTypes
from .. import IrccType
from be_typing import TYPE_CHECKING


class IrccCDefinition(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)
        self._indent = ''

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        if type._declaration[0]:
            self._out_file.write('/* %s */\n%s;\n\n' % (ir_name, type.format(['%s_' % name, '', '', ''])))

    def begin_method(self, name, return_type, parameters, calling_convention, has_definition):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str, bool) -> bool
        if has_definition:
            self._out_file.write(
                '%s %s(%s)\n{\n' % (
                    return_type.format(['', '', '', '']
                                       ), name, ', '.join(t.format(['', '', n, '']) for t, n in parameters)
                )
            )
            self._indent = '    '
            return True
        else:
            return False

    def end_method(self):
        # type: () -> None
        self._indent = ''
        self._out_file.write('}\n\n')

    def declare_variable(self, name, type):
        # type: (str, IrccType) -> None
        self._out_file.write('%s%s;\n' % (self._indent, type.format(['', '', name, ''])))


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType