from .. import IrccGenerator, IrccType
from be_typing import TYPE_CHECKING


class IrccCTypes(IrccGenerator):
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


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType