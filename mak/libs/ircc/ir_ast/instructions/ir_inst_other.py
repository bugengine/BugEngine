from ..ir_code import IrInstruction
from ..ir_type import IrTypeBuiltin, IrTypeVector
from be_typing import TYPE_CHECKING


class IrInstCall(IrInstruction):
    def __init__(self, result, method, arguments, metadata):
        # type: (Optional[IrReference], IrMethod, List[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'call', result, metadata)
        self._method = method
        self._arguments = arguments

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._method.resolve(module)
        for a in self._arguments:
            a.resolve(module, position)
            a.used_by(self)
        return position

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._method.return_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        for a in self._arguments:
            a.resolve_type(equivalence, return_type, return_position)
        equivalence.merge(self._method.equivalence())
        parameters = self._method.parameters()
        assert len(parameters) == len(self._arguments)
        for argument, parameter in zip(self._arguments, parameters):
            argument.get_type().add_equivalence(
                equivalence, argument.get_position(), parameter.get_type(argument.get_type()), parameter.get_position()
            )

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._method.find_instance([argument.get_type() for argument in self._arguments], equivalence)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        method_name = self._method.find_instance(
            [argument.get_type() for argument in self._arguments], code_context._equivalence
        )
        return generator.make_value_call(
            method_name[1:], [v.create_generator_value(generator, code_context) for v in self._arguments]
        )


class IrInstIntegerCompare(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'icmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation
        if isinstance(self._type, IrTypeVector):
            self._value_type = IrTypeVector(IrTypeBuiltin('i1'), self._type._count) # type: IrType
        else:
            self._value_type = IrTypeBuiltin('i1')

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        self._left_operand.resolve(module, position)
        self._right_operand.resolve(module, position)
        self._left_operand.used_by(self)
        self._right_operand.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._type.add_equivalence(
            equivalence, self.get_position(), self._left_operand.get_type(self._type), self._left_operand.get_position()
        )
        self._type.add_equivalence(
            equivalence, self.get_position(), self._right_operand.get_type(self._type),
            self._right_operand.get_position()
        )
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._value_type

    def is_inline(self):
        # type: () -> bool
        return True

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        if isinstance(self._type, IrTypeVector):
            return generator.make_expression_vector_integer_binary_op(
                self._operation, self._value_type.create_generator_type(generator, code_context._equivalence),
                self._type.create_generator_type(generator, code_context._equivalence),
                self._left_operand.create_generator_value(self._type, generator, code_context),
                self._right_operand.create_generator_value(self._type, generator, code_context)
            )
        else:
            return generator.make_expression_integer_binary_op(
                self._operation, self._type.create_generator_type(generator, code_context._equivalence),
                self._left_operand.create_generator_value(self._type, generator, code_context),
                self._right_operand.create_generator_value(self._type, generator, code_context)
            )


class IrInstFloatCompare(IrInstruction):
    def __init__(self, result, type, left_operand, right_operand, operation, metadata):
        # type: (IrReference, IrType, IrExpression, IrExpression, str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'fcmp', result, metadata)
        self._type = type
        self._left_operand = left_operand
        self._right_operand = right_operand
        self._operation = operation
        if isinstance(self._type, IrTypeVector):
            self._value_type = IrTypeVector(IrTypeBuiltin('i1'), self._type._count) # type: IrType
        else:
            self._value_type = IrTypeBuiltin('i1')

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        self._left_operand.resolve(module, position)
        self._right_operand.resolve(module, position)
        self._left_operand.used_by(self)
        self._right_operand.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._type.add_equivalence(
            equivalence, self.get_position(), self._left_operand.get_type(self._type), self._left_operand.get_position()
        )
        self._type.add_equivalence(
            equivalence, self.get_position(), self._right_operand.get_type(self._type),
            self._right_operand.get_position()
        )
        self._left_operand.resolve_type(equivalence, return_type, return_position)
        self._right_operand.resolve_type(equivalence, return_type, return_position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._value_type

    def is_inline(self):
        # type: () -> bool
        return True

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        if isinstance(self._type, IrTypeVector):
            return generator.make_expression_vector_float_binary_op(
                self._operation, self._value_type.create_generator_type(generator, code_context._equivalence),
                self._type.create_generator_type(generator, code_context._equivalence),
                self._left_operand.create_generator_value(self._type, generator, code_context),
                self._right_operand.create_generator_value(self._type, generator, code_context)
            )
        else:
            return generator.make_expression_float_binary_op(
                self._operation, self._type.create_generator_type(generator, code_context._equivalence),
                self._left_operand.create_generator_value(self._type, generator, code_context),
                self._right_operand.create_generator_value(self._type, generator, code_context)
            )


class IrInstPhi(IrInstruction):
    def __init__(self, result, type, origins, metadata):
        # type: (IrReference, IrType, List[Tuple[IrExpression, str]], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'phi', result, metadata)
        self._type = type
        self._origins = origins

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._type.resolve(module)
        for expr, label in self._origins:
            expr.resolve(module, position)
            expr.used_by(self)
        return position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        for o in self._origins:
            o[0].resolve_type(equivalence, return_type, return_position)
            self._type.add_equivalence(
                equivalence, self._origins[0][0].get_position(), o[0].get_type(self._type), o[0].get_position()
            )

    def is_inline(self):
        # type: () -> bool
        return False

    def collect_phi_exprs(self):
        # type: () -> List[Tuple[IrExpression, str]]
        return self._origins

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._type.create_instance(equivalence)

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccExpression]
        pass


class IrInstSelect(IrInstruction):
    def __init__(self, result, condition, value_true, value_false, metadata):
        # type: (IrReference, IrValue, IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'select', result, metadata)
        self._condition = condition
        self._value_true = value_true
        self._value_false = value_false

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        position = IrInstruction.resolve(self, module, position)
        self._condition.resolve(module, position)
        self._value_true.resolve(module, position)
        self._value_false.resolve(module, position)

        return position

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._value_true.get_type()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._condition.resolve_type(equivalence, return_type, return_position)
        self._value_true.resolve_type(equivalence, return_type, return_position)
        self._value_false.resolve_type(equivalence, return_type, return_position)
        self._value_true.get_type().add_equivalence(
            equivalence, self._value_true.get_position(), self._value_false.get_type(), self._value_false.get_position()
        )

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        condition = self._condition.create_generator_value(generator, code_context)
        value_1 = self._value_true.create_generator_value(generator, code_context)
        value_2 = self._value_false.create_generator_value(generator, code_context)
        return generator.make_expression_select(condition, value_1, value_2)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from ..ir_module import IrModule
    from ..ir_expr import IrExpression
    from ..ir_value import IrValue
    from ..ir_reference import IrReference
    from ..ir_method import IrMethod
    from ..ir_metadata import IrMetadataLink
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference
    from ..ir_code import IrCodeGenContext, IrCodeSegment
    from ...ir_codegen import IrccGenerator, IrccExpression
    from ...ir_position import IrPosition