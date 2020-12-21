from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstRet(IrInstruction):
    def __init__(self, return_value, metadata):
        # type: (Optional[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'ret', None, metadata)
        self._return_value = return_value

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        if self._return_value is not None:
            self._return_value = self._return_value.resolve(module)
        return self

    def terminal(self):
        # type: () -> bool
        return True

    def _get_type(self, signature):
        # type: (str) -> Optional[IrType]
        if self._return_value is not None:
            return self._return_value.get_type(signature)
        else:
            return None


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

    def _get_type(self, signature):
        # type: (str) -> Optional[IrType]
        return None


class IrInstConditionalBranch(IrInstruction):
    def __init__(self, condition, target_true, target_false, metadata):
        # type: (IrValue, str, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, '?br', None, metadata)
        self._condition = condition
        self._target_true = target_true
        self._target_false = target_false

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._condition = self._condition.resolve(module)
        return self

    def terminal(self):
        # type: () -> bool
        return True

    def labels(self):
        # type: () -> List[str]
        return [self._target_true[1:], self._target_false[1:]]

    def _get_type(self, signature):
        # type: (str) -> Optional[IrType]
        return None


class IrInstSwitch(IrInstruction):
    def __init__(self, condition, default_label, targets, metadata):
        # type: (IrValue, str, List[Tuple[IrValue, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'switch', None, metadata)
        self._condition = condition
        self._default_label = default_label
        self._targets = targets

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._condition = self._condition.resolve(module)
        self._targets = [(value.resolve(module), label) for value, label in self._targets]
        return self

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
    from ..ir_type import IrType