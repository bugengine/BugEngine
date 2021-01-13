from ..ir_code import IrInstruction
from ..ir_type import IrTypeVector
from be_typing import TYPE_CHECKING


class IrInstExtractElement(IrInstruction):
    def __init__(self, result, value, index, metadata):
        # type: (IrReference, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'extractelement', result, metadata)
        self._value = value
        self._index = index

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._value.resolve(module, position)
        self._index.resolve(module, position)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._value.resolve_type(equivalence, return_type, return_position)
        self._index.resolve_type(equivalence, return_type, return_position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        vector_type = self._value.get_type()
        assert isinstance(vector_type, IrTypeVector)
        return vector_type._type


class IrInstInsertElement(IrInstruction):
    def __init__(self, result, value, element, index, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'insertelement', result, metadata)
        self._value = value
        self._element = element
        self._index = index

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._value.resolve(module, position)
        self._element.resolve(module, position)
        self._index.resolve(module, position)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._value.resolve_type(equivalence, return_type, return_position)
        self._element.resolve_type(equivalence, return_type, return_position)
        self._index.resolve_type(equivalence, return_type, return_position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._value.get_type()


class IrInstShuffleVector(IrInstruction):
    def __init__(self, result, vector_1, vector_2, mask, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'shufflevector', result, metadata)
        self._vector_1 = vector_1
        self._vector_2 = vector_2
        self._mask = mask

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._vector_1.resolve(module, position)
        self._vector_2.resolve(module, position)
        self._mask.resolve(module, position)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._vector_1.resolve_type(equivalence, return_type, return_position)
        self._vector_2.resolve_type(equivalence, return_type, return_position)
        self._mask.resolve_type(equivalence, return_type, return_position)
        self._vector_1.get_type().add_equivalence(
            equivalence, self._vector_1.get_position(), self._vector_2.get_type(), self._vector_2.get_position()
        )

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._vector_1.get_type()


if TYPE_CHECKING:
    from typing import List, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition