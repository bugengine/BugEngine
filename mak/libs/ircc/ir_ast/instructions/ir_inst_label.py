from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstLabel(IrInstruction):
    def __init__(self, label, metadata):
        # type: (str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'label', None, metadata)
        self._label = label


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_metadata import IrMetadataLink