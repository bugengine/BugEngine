from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstFloatUnaryOp(IrInstruction):
    def __init__(self, result, type, operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'float_unary_op', result, metadata)
        self._type = type
        self._operand = operand
        self._operation = operation

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        self._operand.resolve(module, position)
        self._operand.used_by(self)
        return position

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._type.add_equivalence(
            equivalence, self.get_position(), self._operand.get_type(self._type), self._operand.get_position()
        )
        self._operand.resolve_type(equivalence, return_type, return_position)

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._type.create_instance(equivalence)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition