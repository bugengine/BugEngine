from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstFloatUnaryOp(IrInstruction):
    def __init__(self, result, type, operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_unary_op', result, metadata)
        self._type = type
        self._operand = operand
        self._operation = operation


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_type import IrType
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference