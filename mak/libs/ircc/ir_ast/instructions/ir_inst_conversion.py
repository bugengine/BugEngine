from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstCast(IrInstruction):
    def __init__(self, result, value, result_type, cast_type, metadata):
        # type: (IrReference, IrValue, IrType, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'cast', result, metadata)
        self._value = value
        self._result_type = result_type
        self._cast_type = cast_type


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_type import IrType
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference