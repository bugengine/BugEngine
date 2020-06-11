from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstCall(IrInstruction):
    def __init__(self, return_value, metadata):
        # type: (Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'call', None, metadata)


class IrInstIntegerCompare(IrInstruction):
    def __init__(self, type, left_operand, right_operand, operation, result, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'icmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation


class IrInstFloatCompare(IrInstruction):
    def __init__(self, type, left_operand, right_operand, operation, result, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'fcmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation


class IrInstPhi(IrInstruction):
    def __init__(self, result, type, origins, metadata):
        # type: (IrReference, IrType, List[Tuple[IrExpression, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'phi', result, metadata)
        self._type = type
        self._origins = origins


class IrInstSelect(IrInstruction):
    def __init__(self, result, condition, value_true, value_false, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'select', result, metadata)
        self._condition = condition
        self._value_true = value_true
        self._value_false = value_false


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_type import IrType
    from ..ir_expr import IrExpression
    from ..ir_value import IrValue
    from ..ir_reference import IrReference
    from ..ir_metadata import IrMetadataLink