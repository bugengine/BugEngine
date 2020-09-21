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

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._left_operand = self._left_operand.resolve(module)
        self._right_operand = self._right_operand.resolve(module)
        return self


class IrInstFloatBinaryOp(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_binary_op', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._left_operand = self._left_operand.resolve(module)
        self._right_operand = self._right_operand.resolve(module)
        return self


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_type import IrType
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule