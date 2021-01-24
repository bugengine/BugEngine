from .. import IrccGenerator, IrccType
from be_typing import TYPE_CHECKING

_UNSIGNED_VERSIONS = {'i8': 'u8', 'i16': 'u16', 'i32': 'u32', 'i64': 'u64'}


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
        return IrccType(['', '', builtin, ''], ['', '', _UNSIGNED_VERSIONS.get(builtin, builtin), ''])

    def type_declared(self, name):
        # type: (str) -> IrccType
        return IrccType(['', '', name, ''])

    def make_array(self, type, size):
        # type: (IrccType, int) -> IrccType
        return IrccType(['', '', '', '[%d]' % (size)], base=type, sep='')

    def make_ptr(self, type):
        # type: (IrccType) -> IrccType
        return IrccType(['', '', '*', ''], base=type, sep='')

    def make_struct(self, fields):
        # type: (List[Tuple[IrccType, str]]) -> IrccType
        struct_body = ' '.join('%s;' % type.format(['', '', name, '']) for type, name in fields)
        return IrccType(['struct', '{ %s }' % struct_body, '', ''])

    def make_const(self, type):
        # type: (IrccType) -> IrccType
        return IrccType(['', '', 'const', ''], ['', '', 'const', ''], type)

    def make_address_space(self, type, address_space):
        # type: (IrccType, int) -> IrccType
        return type

    def make_vector(self, type, count):
        # type: (IrccType, int) -> IrccType
        base_type = type._declaration[2]
        base_type_unsigned = type._unsigned_declaration[2]
        assert base_type in ['i8', 'i16', 'i32', 'i64', 'float']
        assert count in [2, 4, 8, 16]
        assert type._declaration[0] == ''
        assert type._declaration[1] == ''
        assert type._declaration[3] == ''
        return IrccType(['', '', '%s_%d' % (base_type, count), ''], ['', '', '%s_%d' % (base_type_unsigned, count), ''])


if TYPE_CHECKING:
    from typing import List, TextIO, Tuple
    from ircc import IrccType