from ..ir_code import IrInstruction
from be_typing import TYPE_CHECKING


class IrInstCast(IrInstruction):
    def __init__(self, result, value, result_type, cast_type, metadata):
        # type: (IrReference, IrValue, IrType, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'cast', result, metadata)
        self._value = value
        self._result_type = result_type
        self._cast_type = cast_type

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._value.resolve(module, position)
        self._result_type.resolve(module)
        self._value.used_by(self)
        return position

    def get_type(self):
        # type: () -> IrType
        return self._result_type

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        value_type = self._value.get_type()
        if self._cast_type not in ('bitcast', 'inttoptr', 'zext'):
            self._result_type.add_equivalence(equivalence, self.get_position(), value_type, self._value.get_position())
        elif self._cast_type == 'bitcast':
            value_type.add_equivalence_nonrecursive(
                equivalence, self._value.get_position(), self._result_type, self.get_position()
            )
        self._value.resolve_type(equivalence, return_type, return_position)


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ...ir_position import IrPosition