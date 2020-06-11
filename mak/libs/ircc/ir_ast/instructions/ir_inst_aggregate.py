from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstExtractValue(IrInstruction):
    def __init__(self, result, source, indices, metadata):
        # type: (IrReference, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'extractvalue', result, metadata)
        self._source = source
        self._indices = indices


class IrInstInsertValue(IrInstruction):
    def __init__(self, result, source, value, indices, metadata):
        # type: (IrReference, IrValue, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'insertvalue', result, metadata)
        self._source = source
        self._value = value
        self._indices = indices


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference