from ircc import IrCodeGenerator
from be_typing import TYPE_CHECKING


class ClGenerator(IrCodeGenerator):
    def __init__(self, file):
        # type: (TextIO) -> None
        self._out_file = file

    def begin_module(self):
        # type: () -> None
        self._out_file.write('/* generated code; do not edit */\n\n')

    def end_module(self):
        # type: () -> None
        self._out_file.write('kernel void _kmain()\n')
        self._out_file.write('{\n')
        self._out_file.write('}\n')


if TYPE_CHECKING:
    from typing import TextIO