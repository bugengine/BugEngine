from .ircc_c_types import IrccCTypes
from .. import IrccType
from be_typing import TYPE_CHECKING


class IrccCDefinition(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)

    def declare_type(self, type, name, ir_name):
        # type: (IrccType, str, str) -> None
        if type._declaration[0]:
            self._out_file.write('/* %s */\n%s;\n\n' % (ir_name, type.format(['%s_' % name, '', '', ''])))

    def begin_module(self):
        # type: () -> None
        pass

    def begin_method(self, name, return_type, parameters, calling_convention):
        # type: (str, IrccType, List[Tuple[IrccType, str]], str) -> None
        self._out_file.write(
            '%s %s(%s)\n{\n' %
            (return_type.format(['', '', '', '']), name, ', '.join(t.format(['', '', n, '']) for t, n in parameters))
        )

    def end_method(self):
        # type: () -> None
        self._out_file.write('}\n\n')


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType