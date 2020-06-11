from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstAlloca(IrInstruction):
    def __init__(self, result, type, count, alignment, metadata):
        # type: (IrReference, IrType, Optional[IrValue], Optional[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'alloca', result, metadata)


class IrInstLoad(IrInstruction):
    def __init__(self, result, source, type, metadata):
        # type: (IrReference, IrValue, IrType, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'load', result, metadata)
        self._source = source
        self._type = type


class IrInstStore(IrInstruction):
    def __init__(self, target, value, metadata):
        # type: (IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'store', None, metadata)
        self._target = target
        self._value = value


class IrInstGetElementPtr(IrInstruction):
    def __init__(self, result, type, access, metadata):
        # type: (IrReference, IrType, List[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'getelementptr', result, metadata)
        self._type = type
        self._access = access


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_type import IrType
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference