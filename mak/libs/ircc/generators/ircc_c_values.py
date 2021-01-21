from .ircc_c_types import IrccCTypes
from ..ir_codegen import IrccValue
from be_typing import TYPE_CHECKING


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

    def make_value_undef(self, width):
        # type: (int) -> IrccValue
        return IrccValue('0x%s' % ('cd' * width))

    def make_value_bool(self, value):
        # type: (bool) -> IrccValue
        if value:
            return IrccValue('1')
        else:
            return IrccValue('0')

    def make_value_int(self, value):
        # type: (int) -> IrccValue
        return IrccValue(str(value))

    def make_value_aggregate(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])))

    def make_value_vector(self, values):
        # type: (List[IrccValue]) -> IrccValue
        raise NotImplementedError

    def make_value_array(self, values):
        # type: (List[IrccValue]) -> IrccValue
        return IrccValue('{ %s }' % (', '.join([v._value for v in values])))

    def make_value_cast(self, value, cast, result_type):
        # type: (IrccValue, str, IrccType) -> IrccValue
        return IrccValue('(%s)(%s)' % (result_type.format([]), value._value))


if TYPE_CHECKING:
    from typing import List, TextIO
    from ..ir_codegen import IrccType