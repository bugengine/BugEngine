from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstExtractValue(IrInstruction):
    def __init__(self, result, source, indices, metadata):
        # type: (IrReference, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'extractvalue', result, metadata)
        self._source = source
        self._indices = indices
        self._value_type = None    # type: Optional[IrType]

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._source.resolve(module, position)
        self._source.used_by(self)
        return position

    def get_type(self):
        # type: () -> IrType
        if self._value_type is None:
            value_type = self._source.get_type()
            for i in self._indices:
                value_type = value_type.extract(i)
            self._value_type = value_type
        return self._value_type

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        # since value_type is deduced from the target type, no equivalence is needed
        self._source.resolve_type(equivalence, return_type, return_position)


class IrInstInsertValue(IrInstruction):
    def __init__(self, result, source, value, indices, metadata):
        # type: (IrReference, IrValue, IrValue, List[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'insertvalue', result, metadata)
        self._source = source
        self._value = value
        self._indices = indices
        self._value_type = None    # type: Optional[IrType]

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._source.resolve(module, position)
        self._value.resolve(module, position)
        self._source.used_by(self)
        self._value.used_by(self)
        return position

    def get_type(self):
        # type: () -> IrType
        return self._source.get_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        assert self._value_type is not None
        self._source.resolve_type(equivalence, return_type, return_position)
        self._value.resolve_type(equivalence, return_type, return_position)
        subfield_type = self._value_type
        for i in self._indices:
            subfield_type = subfield_type.extract(i)
        insert_type = self._value.get_type()
        subfield_type.add_equivalence(equivalence, self.get_position(), insert_type, self._source.get_position())


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition