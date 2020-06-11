
from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstRet(IrInstruction):
    def __init__(self, return_value, metadata):
        # type: (Optional[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'ret', None, metadata)
        self._return_value = return_value


class IrInstBranch(IrInstruction):
    def __init__(self, target, metadata):
        # type: (str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'br', None, metadata)
        self._target = target


class IrInstConditionalBranch(IrInstruction):
    def __init__(self, condition, target_true, target_false, metadata):
        # type: (IrValue, str, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, '?br', None, metadata)
        self._condition = condition
        self._target_true = target_true
        self._target_false = target_false


class IrInstSwitch(IrInstruction):
    def __init__(self, condition, default_label, targets, metadata):
        # type: (IrValue, str, List[Tuple[IrValue, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'switch', None, metadata)
        self._condition = condition
        self._default_label = default_label
        self._targets = targets


class IrInstUnreachable(IrInstruction):
    def __init__(self, metadata):
        # type: (List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'unreachable', None, metadata)


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference