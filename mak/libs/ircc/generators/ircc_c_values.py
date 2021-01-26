from .ircc_c_types import IrccCTypes
from ..ir_codegen import IrccValue
from be_typing import TYPE_CHECKING

_SNIPPETS = {'int': {}, 'float': {}, 'vector': {}}


class IrccCValues(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)

    def make_value_void(self):
        # type: () -> IrccValue
        return IrccValue('')

    def make_value_zero(self):
        # type: () -> IrccValue
        return IrccValue('0')

    def make_value_nullptr(self, type):
        # type: (IrccType) -> IrccValue
        return IrccValue('(%s)0' % type.format([]))

    def make_value_undef(self, width):
        # type: (int) -> IrccValue
        return IrccValue('0x%s' % ('cd' * width))

    def make_value_bool(self, type, value):
        # type: (IrccType, bool) -> IrccValue
        if value:
            return IrccValue('1')
        else:
            return IrccValue('0')

    def make_value_int(self, type, value):
        # type: (IrccType, int) -> IrccValue
        return IrccValue('(%s)%s' % (type.format([]), str(value)))

    def make_value_aggregate(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])))

    def make_value_vector(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    def make_value_array(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])))

    def make_value_cast(self, result_type, cast, value):
        # type: (IrccType, str, IrccValue) -> IrccValue
        if cast != 'addrspacecast':
            return IrccValue('(%s)(%s)' % (result_type.format([]), value._value))
        else:
            return value

    def make_value_addressof(self, value):
        # type: (IrccValue) -> IrccValue
        return IrccValue('&(%s)' % value._value)

    def make_value_access(self, value, field_name):
        # type: (IrccValue, str) -> IrccValue
        return IrccValue('(%s).%s' % (value._value, field_name))

    def make_value_index(self, value, index):
        # type: (IrccValue, IrccValue) -> IrccValue
        return IrccValue('(%s)[%s]' % (value._value, index._value))

    def make_value_reference(self, name):
        # type: (str) -> IrccValue
        return IrccValue(name)

    def make_value_binary_op(self, operation, left_operand, right_operand):
        # type: (str, IrccValue, IrccValue) -> IrccValue
        return IrccValue('%s %s %s' % (left_operand._value, operation, right_operand._value))

    def make_value_load(self, address):
        # type: (IrccValue) -> IrccValue
        if address._value[0] == '&':
            return IrccValue(address._value[2:-1])
        else:
            return IrccValue('*(%s)' % (address._value))

    def make_value_call(self, method_name, arguments):
        # type: (str, List[IrccValue]) -> IrccValue
        return IrccValue('%s(%s)' % (method_name, ', '.join([a._value for a in arguments])))


if TYPE_CHECKING:
    from typing import List, TextIO
    from ..ir_codegen import IrccType