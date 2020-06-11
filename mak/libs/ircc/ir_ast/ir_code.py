from be_typing import TYPE_CHECKING


class IrInstruction:
    def __init__(self, opcode, result, metadata):
        # type: (str, Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        self._opcode = opcode
        self._result = result
        self._metadata = metadata


class IrCodeSegment:
    def __init__(self, label, instructions):
        # type: (str, List[IrInstruction]) -> None
        self._label = label
        self._instructions = instructions


class IrCodeBlock:
    def __init__(self, instructions):
        # type: (List[IrInstruction]) -> None
        self._segments = []
        label = 'start'
        stream = []    # type: List[IrInstruction]
        for instruction in instructions:
            if instruction._opcode == 'label':
                assert len(stream) == 0
                pass
            else:
                stream.append(instruction)
                if instruction._opcode in ('ret', 'br', 'switch'):
                    self._segments.append(IrCodeSegment(label, stream))


if TYPE_CHECKING:
    from typing import List, Optional, Tuple, Union
    from .ir_metadata import IrMetadataLink
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_expr import IrExpression
    from .ir_reference import IrReference