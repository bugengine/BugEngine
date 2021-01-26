from .ircc_c_types import IrccCTypes
from ..ir_codegen import IrccValue
from be_typing import TYPE_CHECKING

_SNIPPETS = {'int': {}, 'float': {}, 'vector': {}}

_PREC_VALUE = 0
_PREC_ARRAY = 1
_PREC_CALL = 1
_PREC_INDEX = 1
_PREC_ACCESS = 1
_PREC_CAST = 2
_PREC_ADDRESSOF = 2
_PREC_DEREFERENCE = 2

_PREC_BINOP = 12   # TODO


class IrccCValues(IrccCTypes):
    def __init__(self, file):
        # type: (TextIO) -> None
        IrccCTypes.__init__(self, file)

    def make_value_void(self):
        # type: () -> IrccValue
        return IrccValue('', _PREC_VALUE)

    def make_value_zero(self):
        # type: () -> IrccValue
        return IrccValue('0', _PREC_VALUE)

    def make_value_nullptr(self, type):
        # type: (IrccType) -> IrccValue
        return IrccValue('(%s)0' % type.format([]), _PREC_VALUE)

    def make_value_undef(self, width):
        # type: (int) -> IrccValue
        return IrccValue('0x%s' % ('cd' * width), _PREC_VALUE)

    def make_value_bool(self, type, value):
        # type: (IrccType, bool) -> IrccValue
        if value:
            return IrccValue('1', _PREC_VALUE)
        else:
            return IrccValue('0', _PREC_VALUE)

    def make_value_int(self, type, value):
        # type: (IrccType, int) -> IrccValue
        return IrccValue('(%s)%s' % (type.format([]), str(value)), _PREC_VALUE)

    def make_value_aggregate(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])), _PREC_VALUE)

    def make_value_vector(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    def make_value_array(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])), _PREC_VALUE)

    def make_value_cast(self, result_type, cast, value):
        # type: (IrccType, str, IrccValue) -> IrccValue
        if cast != 'addrspacecast':
            return IrccValue('(%s)%s' % (result_type.format([]), value.wrap(_PREC_CAST)), _PREC_CAST)
        else:
            return value

    def make_value_addressof(self, value):
        # type: (IrccValue) -> IrccValue
        return IrccValue('&%s' % value.wrap(_PREC_ADDRESSOF), _PREC_ADDRESSOF)

    def make_value_access(self, value, field_name):
        # type: (IrccValue, str) -> IrccValue
        return IrccValue('%s.%s' % (value.wrap(_PREC_ACCESS), field_name), _PREC_ACCESS)

    def make_value_index(self, value, index):
        # type: (IrccValue, IrccValue) -> IrccValue
        return IrccValue('%s[%s]' % (value.wrap(_PREC_INDEX), index._value), _PREC_INDEX)

    def make_value_reference(self, name):
        # type: (str) -> IrccValue
        return IrccValue(name, _PREC_VALUE)

    def make_value_binary_op(self, operation, left_operand, right_operand):
        # type: (str, IrccValue, IrccValue) -> IrccValue
        return IrccValue(
            '%s %s %s' % (left_operand.wrap(_PREC_BINOP), operation, right_operand.wrap(_PREC_BINOP)), _PREC_BINOP
        )

    def make_value_load(self, address):
        # type: (IrccValue) -> IrccValue
        return IrccValue('*%s' % (address.wrap(_PREC_DEREFERENCE)), _PREC_DEREFERENCE)

    def make_value_call(self, method_name, arguments):
        # type: (str, List[IrccValue]) -> IrccValue
        return IrccValue('%s(%s)' % (method_name, ', '.join([a._value for a in arguments])), _PREC_CALL)


if TYPE_CHECKING:
    from typing import List, TextIO
    from ..ir_codegen import IrccType