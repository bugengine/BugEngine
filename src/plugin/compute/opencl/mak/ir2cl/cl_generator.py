from ircc import IrCodeGenerator
from be_typing import TYPE_CHECKING


class ClGenerator(IrCodeGenerator):
    def __init__(self, file):
        # type: (TextIO) -> None
        self._out_file = file
        self._address_spaces = {
            0: '',
            1: '__global',
            2: '__constant',
            3: '__local',
            4: '__todo'
        }

    def type_void(self):
        # type: () -> str
        return 'void'

    def type_builtin(self, builtin):
        # type: (str) -> str
        # typedefs in the source map the builtins
        return builtin

    def type_declared(self, name):
        # type: (str) -> str
        return name

    def make_array(self, type, size):
        # type: (str, int) -> str
        return '%s[%d]' % (type, size)

    def make_ptr(self, type):
        # type: (str) -> str
        return type + '*'

    def make_struct(self, fields):
        # type: (List[Tuple[str, str]]) -> str
        return 'struct { %s }' % (' '.join('%s %s;' % (t, n) for t, n in fields))

    def make_const(self, type):
        # type: (str) -> str
        return type + ' const'

    def make_address_space(self, type, address_space):
        # type: (str, int) -> str
        if address_space:
            return '%s %s' % (type, self._address_spaces[address_space])
        else:
            return type
    
    def begin_module(self):
        # type: () -> None
        self._out_file.write('/* generated code; do not edit */\n'
                             'typedef signed char  i8;\n'
                             'typedef signed short i16;\n'
                             'typedef signed int   i32;\n'
                             'typedef signed long  i64;\n'
                             'typedef void* metadata;\n'
                             '#if 0\n')

    def header_specifier(self, name, value):
        # type: (str, str) -> None
        if name == 'source_filename':
            self._out_file.write('/* generated from %s */\n' % value)

    def end_module(self):
        # type: () -> None
        self._out_file.write('#endif\n'
                             'kernel void _kmain()\n'
                             '{\n'
                             '}\n')

    def declare_type(self, type, name, ir_name):
        # type: (str, str) -> None
        self._out_file.write('/* %s */\ntypedef %s %s;\n\n' % (ir_name, type, name))

    def begin_method(self, name, return_type, parameters):
        # type: (str, str, List[Tuple[str, str]]) -> None
        self._out_file.write('%s %s(%s)\n{\n' % (return_type, name, ', '.join('%s %s' % (t, n) for t, n in parameters)))

    def end_method(self):
        # type: () -> None
        self._out_file.write('}\n')

if TYPE_CHECKING:
    from typing import List, TextIO, Tuple