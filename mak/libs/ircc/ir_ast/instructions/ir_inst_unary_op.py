from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstFloatUnaryOp(IrInstruction):
    def __init__(self, result, type, operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_unary_op', result, metadata)
        self._type = type
        self._operand = operand
        self._operation = operation

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._operand = self._operand.resolve(module)
        return self


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_type import IrType
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule