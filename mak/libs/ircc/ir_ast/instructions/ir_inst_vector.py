from ..ir_code import IrInstruction
from ..ir_type import IrTypeVector
from ..ir_value import IrValueExpr
from ..ir_expr import IrExpressionConstant, IrExpressionReference, IrExpressionUndef, IrExpressionZero, IrExpressionVector
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
        self._value.used_by(self)
        self._index.used_by(self)
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

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        if isinstance(self._index, IrValueExpr) and isinstance(self._index._expression, IrExpressionConstant):
            assert isinstance(self._index._expression._value, int)
            return generator.make_expression_vector_extract_constant(
                self._value.create_generator_value(generator, code_context), self._index._expression._value
            )
        else:
            vector_type = self._value.get_type()
            assert isinstance(vector_type, IrTypeVector)
            return generator.make_expression_vector_extract(
                vector_type._type.create_generator_type(generator, code_context._equivalence),
                self._value.create_generator_value(generator, code_context),
                self._index.create_generator_value(generator, code_context)
            )


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
        self._value.used_by(self)
        self._element.used_by(self)
        self._index.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._value.resolve_type(equivalence, return_type, return_position)
        self._element.resolve_type(equivalence, return_type, return_position)
        self._index.resolve_type(equivalence, return_type, return_position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._value.get_type()

    def is_inline(self):
        # type: () -> bool
        if isinstance(self._value, IrValueExpr):
            expr = self._value._expression
            if isinstance(expr, IrExpressionReference):
                if expr._reference[0] == '%':
                    assert isinstance(expr._expression, IrInstruction)
                    if len(expr._expression._usage) == 1:
                        assert self in expr._expression._usage
                        return True
        return False

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        if self.is_inline():
            return self._value.create_generator_value(generator, code_context)
        else:
            assert self._result_name is not None
            return generator.make_expression_variable_reference(self._result_name)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        assert self._result_name is not None
        result_name = self._result_name
        if not self._value.is_undef():
            if self.is_inline():
                assert isinstance(self._value, IrValueExpr)
                assert isinstance(self._value._expression, IrExpressionReference)
                assert isinstance(self._value._expression._expression, IrInstruction)
                assert self._value._expression._expression._result_name is not None
                result_name = self._value._expression._expression._result_name
            else:
                generator.instruction_assign(
                    generator.make_expression_variable_reference(result_name),
                    self._value.create_generator_value(generator, context)
                )
        if isinstance(self._index, IrValueExpr) and isinstance(self._index._expression, IrExpressionConstant):
            assert isinstance(self._index._expression._value, int)
            generator.instruction_vector_insert_constant(
                generator.make_expression_variable_reference(result_name), self._index._expression._value,
                self._element.create_generator_value(generator, context)
            )
        else:
            vector_type = self._value.get_type()._get_target_type()
            assert isinstance(vector_type, IrTypeVector)
            generator.instruction_vector_insert(
                vector_type._type.create_generator_type(generator, context._equivalence),
                generator.make_expression_variable_reference(result_name),
                self._index.create_generator_value(generator, context),
                self._element.create_generator_value(generator, context)
            )
        return None


class IrInstShuffleVector(IrInstruction):
    def __init__(self, result, vector_1, vector_2, mask, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'shufflevector', result, metadata)
        self._vector_1 = vector_1
        self._vector_2 = vector_2
        assert isinstance(self._vector_1._type, IrTypeVector)
        self._vector_len = self._vector_1._type._count
        # mask needs to be a list of constant indices, zeroinitializer, or undefs
        assert isinstance(mask, IrValueExpr)
        assert isinstance(mask._type, IrTypeVector)
        mask_len = mask._type._count
        mask_expr = mask._expression
        self._result_type = IrTypeVector(self._vector_1._type._type, mask_len)
        if isinstance(mask_expr, IrExpressionZero):
            self._mask = [0] * mask_len
        elif isinstance(mask_expr, IrExpressionUndef):
            self._mask = [0] * mask_len
        else:
            self._mask = []
            assert isinstance(mask_expr, IrExpressionVector)
            for value in mask_expr._values:
                assert isinstance(value, IrValueExpr)
                assert isinstance(value._expression, IrExpressionConstant)
                assert isinstance(value._expression._value, int)
                self._mask.append(value._expression._value)

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._vector_1.resolve(module, position)
        self._vector_2.resolve(module, position)
        for index in self._mask:
            if index < self._vector_len:
                self._vector_1.used_by(self)
            else:
                self._vector_2.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._vector_1.resolve_type(equivalence, return_type, return_position)
        self._vector_2.resolve_type(equivalence, return_type, return_position)
        self._vector_1.get_type().add_equivalence(
            equivalence, self._vector_1.get_position(), self._vector_2.get_type(), self._vector_2.get_position()
        )

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._vector_1.get_type()

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_vector_shuffle(
            self._result_type.create_generator_type(generator, code_context._equivalence), self._vector_len,
            self._vector_1.create_generator_value(generator, code_context),
            self._vector_2.create_generator_value(generator, code_context), self._mask
        )


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ..ir_code import IrCodeGenContext, IrCodeSegment
    from ...ir_codegen import IrccGenerator, IrccExpression
    from ...ir_position import IrPosition