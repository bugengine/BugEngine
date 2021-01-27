from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstLabel(IrInstruction):
    def __init__(self, label, metadata):
        # type: (str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'label', None, metadata)
        self._label = label

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        generator.declare_label(self._label)
        return None


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_metadata import IrMetadataLink
    from ..ir_code import IrCodeGenContext, IrCodeSegment
    from ...ir_codegen import IrccGenerator, IrccExpression