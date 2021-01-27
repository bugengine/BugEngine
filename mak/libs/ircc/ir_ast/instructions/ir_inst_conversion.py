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

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
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

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._result_type.create_instance(equivalence)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass

    def is_inline(self):
        # type: () -> bool
        return True

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        value = self._value.create_generator_value(generator, code_context)
        target_type = self._result_type.create_generator_type(generator, code_context._equivalence)
        return generator.make_expression_cast(target_type, self._cast_type, value)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ..ir_code import IrCodeGenContext, IrCodeSegment
    from ...ir_codegen import IrccGenerator, IrccExpression
    from ...ir_position import IrPosition