from ..  import IrccGenerator, IrccType
from be_typing import TYPE_CHECKING


class IrccCDeclaration(IrccGenerator):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccGenerator.__init__(self, file)

    def type_void(self):
        # type: () -> IrccType
        return IrccType(['', '', 'void', ''])

    def type_builtin(self, builtin):
        # type: (str) -> IrccType
        # typedefs in the source map the builtins
        return IrccType(['', '', builtin, ''])

    def type_declared(self, name):
        # type: (str) -> IrccType
        return IrccType(['', '', name, ''])

    def make_array(self, type, size):
        # type: (IrccType, int) -> IrccType
        return IrccType(['', '', '', '[%d]' % (size)], type, sep='')

    def make_ptr(self, type):
        # type: (IrccType) -> IrccType
        return IrccType(['', '', '*', ''], type, sep='')

    def make_struct(self, fields):
        # type: (List[Tuple[IrccType, str]]) -> IrccType
        struct_body = ' '.join('%s;' % type.format(['', '', name, '']) for type, name in fields)
        return IrccType(['struct', '{ %s }' % struct_body, '', ''])

    def make_const(self, type):
        # type: (IrccType) -> IrccType
        return IrccType(['', '', 'const', ''], type)

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return type

    def begin_module(self):
        # type: () -> None
        self._out_file.write('/* generated code; do not edit */\n'
                             '#include <stdint.h>\n'
                             'typedef int8_t  i8;\n'
                             'typedef int16_t i16;\n'
                             'typedef int32_t i32;\n'
                             'typedef int64_t i64;\n'
                             'typedef void* metadata;\n'
                             '\n')

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

    def begin_method(self, name, return_type, parameters):
        # type: (str, str, List[Tuple[str, str]]) -> None
        self._out_file.write('%s %s(%s)' % (return_type, name, ', '.join('%s' % t.format(['', '', name, '']) for t, n in parameters)))

    def end_method(self):
        # type: () -> None
        self._out_file.write(';\n')

if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType