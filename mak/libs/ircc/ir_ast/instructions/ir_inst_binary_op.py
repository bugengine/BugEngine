from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstBinaryOp(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'binary_op', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation


class IrInstFloatBinaryOp(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_binary_op', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_type import IrType
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference