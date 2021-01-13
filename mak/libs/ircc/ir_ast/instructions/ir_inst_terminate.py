from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstRet(IrInstruction):
    def __init__(self, return_value, metadata):
        # type: (IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'ret', None, metadata)
        self._return_value = return_value

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._return_value.resolve(module, position)
        self._return_value.used_by(self)
        return position

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._return_value.get_type()

    def terminal(self):
        # type: () -> bool
        return True

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        if self._return_value is not None:
            self._return_value.resolve_type(equivalence, return_type, return_position)
            return_value_type = self._return_value.get_type()
            assert return_value_type is not None
            assert return_type is not None
            return_type.add_equivalence(equivalence, return_position, return_value_type, self.get_position())


class IrInstBranch(IrInstruction):
    def __init__(self, target, metadata):
        # type: (str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'br', None, metadata)
        self._target = target

    def terminal(self):
        # type: () -> bool
        return True

    def labels(self):
        # type: () -> List[str]
        return [self._target[1:]]


class IrInstConditionalBranch(IrInstruction):
    def __init__(self, condition, target_true, target_false, metadata):
        # type: (IrValue, str, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, '?br', None, metadata)
        self._condition = condition
        self._target_true = target_true
        self._target_false = target_false

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._condition.resolve(module, position)
        self._condition.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._condition.resolve_type(equivalence, return_type, return_position)

    def terminal(self):
        # type: () -> bool
        return True

    def labels(self):
        # type: () -> List[str]
        return [self._target_true[1:], self._target_false[1:]]


class IrInstSwitch(IrInstruction):
    def __init__(self, condition, default_label, targets, metadata):
        # type: (IrValue, str, List[Tuple[IrValue, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'switch', None, metadata)
        self._condition = condition
        self._default_label = default_label
        self._targets = targets

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._condition.resolve(module, position)
        self._condition.used_by(self)
        for value, label in self._targets:
            value.resolve(module, position)
            value.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._condition.resolve_type(equivalence, return_type, return_position)
        for value, label in self._targets:
            value.resolve_type(equivalence, return_type, return_position)

    def terminal(self):
        # type: () -> bool
        return True

    def labels(self):
        # type: () -> List[str]
        return [self._default_label[1:]] + [t[1:] for _, t in self._targets]


class IrInstUnreachable(IrInstruction):
    def __init__(self, metadata):
        # type: (List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'unreachable', None, metadata)

    def terminal(self):
        # type: () -> bool
        return True


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition