from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstExtractValue(IrInstruction):
    def __init__(self, result, source, indices, metadata):
        # type: (IrReference, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'extractvalue', result, metadata)
        self._source = source
        self._indices = indices

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._source = self._source.resolve(module)
        value_type = self._source.get_type()[0]
        for i in self._indices:
            value_type = value_type.extract(i)
        self._value_type = value_type
        return self


class IrInstInsertValue(IrInstruction):
    def __init__(self, result, source, value, indices, metadata):
        # type: (IrReference, IrValue, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'insertvalue', result, metadata)
        self._source = source
        self._value = value
        self._indices = indices

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._source = self._source.resolve(module)
        self._value = self._value.resolve(module)
        self._value_type = self._source.get_type()[0]
        return self


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference