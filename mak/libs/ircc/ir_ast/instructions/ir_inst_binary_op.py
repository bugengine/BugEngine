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

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._type.add_equivalence(
            equivalence, self.get_position(), self._left_operand.get_type(self._type), self._left_operand.get_position()
        )
        self._type.add_equivalence(
            equivalence, self.get_position(), self._right_operand.get_type(self._type),
            self._right_operand.get_position()
        )
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._type.create_instance(equivalence)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccValue]
        pass


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

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._type.add_equivalence(
            equivalence, self.get_position(), self._left_operand.get_type(self._type), self._left_operand.get_position()
        )
        self._type.add_equivalence(
            equivalence, self.get_position(), self._right_operand.get_type(self._type),
            self._right_operand.get_position()
        )
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._type.create_instance(equivalence)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccValue]
        pass


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..ir_expr import IrExpression
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ..ir_code import IrCodeGenContext, IrCodeSegment
    from ...ir_codegen import IrccGenerator, IrccValue
    from ...ir_position import IrPosition