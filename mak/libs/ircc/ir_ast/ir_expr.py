from .ir_object import IrObject
from .ir_type import IrTypeBuiltin, IrTypeUndef, IrTypeZero
from .ir_declaration import IrDeclaration
from ..ir_position import IrPosition
from abc import abstractmethod
from be_typing import TYPE_CHECKING


class IrExpression(IrObject):
    def __init__(self, metadata=[]):
        # type: (List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrObject.__init__(self)
        self._metadata = metadata
        self._metadata_values = {}     # type: Dict[str, IrMetadata]
        self._position = IrPosition('', 0, 0, 0, '')
        self._usage = []               # type: List[IrExpression]

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        for name, value in self._metadata:
            value.resolve(module, position)
        for name, value in self._metadata:
            if name._reference == '!dbg':
                self._position = value.get_position()
                break
        else:
            self._position = position
        return self._position

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def get_position(self):
        # type: () -> IrPosition
        return self._position

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        return

    def used_by(self, expression):
        # type: (IrExpression) -> None
        self._usage.append(expression)

    def is_undef(self):
        # type: () -> bool
        return False

    @abstractmethod
    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        raise NotImplementedError


class IrExpressionDeclaration(IrDeclaration):
    def __init__(self, expression):
        # type: (IrExpression) -> None
        IrDeclaration.__init__(self)
        self._expression = expression

    def resolve(self, module):
        # type: (IrModule) -> None
        self._expression.resolve(module, IrPosition('', 0, 0, 0, ''))


class IrExpressionZero(IrExpression):
    def __init__(self):
        # type: () -> None
        IrExpression.__init__(self)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return suggested_type

    def __str__(self):
        # type: () -> str
        return 'zero'

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return type.create_generator_zero(generator, code_context._equivalence)


class IrExpressionConstant(IrExpression):
    def __init__(self, value):
        # type: (Union[int, str, bool, None]) -> None
        IrExpression.__init__(self)
        self._value = value

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return suggested_type

    def __str__(self):
        # type: () -> str
        return str(self._value)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        if isinstance(self._value, bool):
            return generator.make_expression_constant_bool(
                type.create_generator_type(generator, code_context._equivalence), self._value
            )
        elif isinstance(self._value, int):
            return generator.make_expression_constant_int(
                type.create_generator_type(generator, code_context._equivalence), self._value
            )
        elif self._value is None:
            return generator.make_expression_constant_nullptr(
                type.create_generator_type(generator, code_context._equivalence)
            )
        else:
            raise NotImplementedError


class IrExpressionUndef(IrExpression):
    def __init__(self):
        # type: () -> None
        IrExpression.__init__(self)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return suggested_type

    def __str__(self):
        # type: () -> str
        return 'undefined'

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return type.create_generator_undef(generator, code_context._equivalence)

    def is_undef(self):
        # type: () -> bool
        return True


class IrExpressionArray(IrExpression):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrExpression.__init__(self)
        self._values = values

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        for v in self._values:
            v.resolve(module, position)
        return IrExpression.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        return '[%s]' % ', '.join(str(v) for v in self._values)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_array_value(
            [v.create_generator_value(generator, code_context) for v in self._values]
        )


class IrExpressionVector(IrExpression):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrExpression.__init__(self)
        self._values = values

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        for v in self._values:
            v.resolve(module, position)
        return IrExpression.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        return '<%s>' % ', '.join(str(v) for v in self._values)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_vector_value(
            type.create_generator_type(generator, code_context._equivalence),
            [v.create_generator_value(generator, code_context) for v in self._values]
        )


class IrExpressionAggregate(IrExpression):
    def __init__(self, values):
        # type: (List[IrValue]) -> None
        IrExpression.__init__(self)
        self._values = values

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        for v in self._values:
            v.resolve(module, position)
        return IrExpression.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        return '{%s}' % ', '.join(str(v) for v in self._values)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_aggregate_value(
            [v.create_generator_value(generator, code_context) for v in self._values]
        )


class IrExpressionReference(IrExpression):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrExpression.__init__(self)
        self._reference = reference
        self._expression = None    # type: Optional[IrExpression]

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        if self._expression is None:
            declaration = module.get(self._reference, IrExpressionDeclaration)
            #declaration = declaration.resolve(module)
            self._expression = declaration._expression
        return IrExpression.resolve(self, module, position)

    def get_position(self):
        # type: () -> IrPosition
        assert self._expression is not None
        return self._expression.get_position()

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        assert self._expression is not None
        return self._expression.get_type(suggested_type)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        assert self._expression is not None
        return self._expression.create_generator_value(type, generator, code_context)

    def __str__(self):
        # type: () -> str
        return self._reference

    def used_by(self, expression):
        # type: (IrExpression) -> None
        assert self._expression is not None
        self._expression.used_by(expression)


class IrExpressionCast(IrExpression):
    def __init__(self, result_type, value, cast_type):
        # type: (IrType, IrValue, str) -> None
        IrExpression.__init__(self)
        self._result_type = result_type
        self._value = value
        self._cast_type = cast_type

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        assert self._result_type.is_defined()
        return self._result_type

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        self._result_type.resolve(module)
        self._value.resolve(module, position)
        return IrExpression.resolve(self, module, position)

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        value_type = self._value.get_type()
        if self._cast_type not in ('bitcast', 'inttoptr', 'zext'):
            self._result_type.add_equivalence(equivalence, self.get_position(), value_type, self._value.get_position())
        elif self._cast_type == 'bitcast':
            value_type.add_equivalence_nonrecursive(
                equivalence, self._value.get_position(), self._result_type, self.get_position()
            )

    def __str__(self):
        # type: () -> str
        return '%s_cast<%s>(%s)' % (self._cast_type, self._result_type, self._value)

    def used_by(self, expression):
        # type: (IrExpression) -> None
        self._value.used_by(expression)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_cast(
            self._result_type.create_generator_type(generator, code_context._equivalence), self._cast_type,
            self._value.create_generator_value(generator, code_context)
        )


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from .ir_type import IrType, IrAddressSpaceInference
    from .ir_module import IrModule
    from .ir_value import IrValue
    from .ir_reference import IrReference
    from .ir_metadata import IrMetadata, IrMetadataLink
    from .ir_code import IrCodeGenContext
    from ..ir_codegen import IrccGenerator, IrccExpression
