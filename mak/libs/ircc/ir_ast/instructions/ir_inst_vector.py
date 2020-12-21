from ..ir_code import IrInstruction
from ..ir_type import IrTypeVector
from be_typing import TYPE_CHECKING


class IrInstExtractElement(IrInstruction):
    def __init__(self, result, value, index, metadata):
        # type: (IrReference, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'extractelement', result, metadata)
        self._value = value
        self._index = index

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._value = self._value.resolve(module)
        self._index = self._index.resolve(module)
        return self


class IrInstInsertElement(IrInstruction):
    def __init__(self, result, value, element, index, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'insertelement', result, metadata)
        self._value = value
        self._element = element
        self._index = index

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._value = self._value.resolve(module)
        self._element = self._element.resolve(module)
        self._index = self._index.resolve(module)
        return self


class IrInstShuffleVector(IrInstruction):
    def __init__(self, result, vector_1, vector_2, mask, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'shufflevector', result, metadata)
        self._vector_1 = vector_1
        self._vector_2 = vector_2
        self._mask = mask

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._vector_1 = self._vector_1.resolve(module)
        self._vector_2 = self._vector_2.resolve(module)
        self._mask = self._mask.resolve(module)
        return self


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType
