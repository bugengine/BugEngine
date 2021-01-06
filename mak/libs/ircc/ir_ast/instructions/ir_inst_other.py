from ..ir_code import IrInstruction
from ..ir_type import IrTypeBuiltin, IrTypeVector
from be_typing import TYPE_CHECKING


class IrInstCall(IrInstruction):
    def __init__(self, result, method, arguments, metadata):
        # type: (Optional[IrReference], IrMethod, List[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'call', result, metadata)
        self._method = method
        self._arguments = arguments

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._method = self._method.resolve(module)
        self._arguments = [a.resolve(module) for a in self._arguments]
        return IrInstruction.resolve(self, module)

    def get_type(self):
        # type: () -> IrType
        return self._method.return_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        for a in self._arguments:
            a.resolve_type(equivalence, return_type, return_position)
        equivalence.merge(self._method.equivalence())
        parameters = self._method.parameters()
        assert len(parameters) == len(self._arguments)
        for argument, parameter in zip(self._arguments, parameters):
            argument.get_type().add_equivalence(
                equivalence, argument.get_position(), parameter.get_type(), parameter.get_position()
            )


class IrInstIntegerCompare(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'icmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation
        self._value_type = IrTypeBuiltin('i1')

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._left_operand = self._left_operand.resolve(module)
        self._right_operand = self._right_operand.resolve(module)
        return IrInstruction.resolve(self, module)

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def get_type(self):
        # type: () -> IrType
        return self._value_type


class IrInstFloatCompare(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'fcmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation
        self._value_type = IrTypeBuiltin('i1')

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._left_operand = self._left_operand.resolve(module)
        self._right_operand = self._right_operand.resolve(module)
        return IrInstruction.resolve(self, module)

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def get_type(self):
        # type: () -> IrType
        return self._value_type


class IrInstPhi(IrInstruction):
    def __init__(self, result, type, origins, metadata):
        # type: (IrReference, IrType, List[Tuple[IrExpression, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'phi', result, metadata)
        self._type = type
        self._origins = origins

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._origins = [(expr.resolve(module), label) for expr, label in self._origins]
        return IrInstruction.resolve(self, module)

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        for o in self._origins:
            o[0].resolve_type(equivalence, return_type, return_position)
            self._type.add_equivalence(
                equivalence, self._origins[0][0].get_position(), o[0].get_type(), o[0].get_position()
            )

    def get_type(self):
        # type: () -> IrType
        return self._type


class IrInstSelect(IrInstruction):
    def __init__(self, result, condition, value_true, value_false, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'select', result, metadata)
        self._condition = condition
        self._value_true = value_true
        self._value_false = value_false

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._condition = self._condition.resolve(module)
        self._value_true = self._value_true.resolve(module)
        self._value_false = self._value_false.resolve(module)
        return IrInstruction.resolve(self, module)

    def get_type(self):
        # type: () -> IrType
        return self._value_true.get_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._condition.resolve_type(equivalence, return_type, return_position)
        self._value_true.resolve_type(equivalence, return_type, return_position)
        self._value_false.resolve_type(equivalence, return_type, return_position)
        self._value_true.get_type().add_equivalence(
            equivalence, self._value_true.get_position(), self._value_false.get_type(), self._value_false.get_position()
        )


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_module import IrModule
    from ..ir_expr import IrExpression
    from ..ir_value import IrValue
    from ..ir_reference import IrReference
    from ..ir_method import IrMethod
    from ..ir_metadata import IrMetadataLink
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition