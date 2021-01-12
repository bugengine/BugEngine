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

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        self._left_operand.resolve(module, position)
        self._right_operand.resolve(module, position)
        self._left_operand.used_by(self)
        self._right_operand.used_by(self)
        return position

    def get_type(self):
        # type: () -> IrType
        return self._left_operand.get_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)


class IrInstFloatBinaryOp(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_binary_op', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        self._left_operand.resolve(module, position)
        self._right_operand.resolve(module, position)
        self._left_operand.used_by(self)
        self._right_operand.used_by(self)
        return position

    def get_type(self):
        # type: () -> IrType
        return self._left_operand.get_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition