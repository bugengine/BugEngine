from .ir_object import IrObject
from .ir_type import IrTypeMetadata, IrTypeBuiltin, IrTypeVoid
from ..ir_position import IrPosition
from abc import abstractmethod
from be_typing import TYPE_CHECKING


class IrValue(IrObject):
    def __init__(self, type):
        # type: (IrType) -> None
        IrObject.__init__(self)
        self._type = type

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        self._type = self._type.resolve(module)

    def get_type(self):
        # type: () -> IrType
        raise NotImplementedError

    def get_position(self):
        # type: () -> IrPosition
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        return

    def used_by(self, expression):
        # type: (IrExpression) -> None
        pass

    @abstractmethod
    def create_generator_value(self, generator, code_context):
        # type: (IrccGenerator, IrCodeGenContext) -> IrccExpression
        raise NotImplementedError


class IrValueExpr(IrValue):
    def __init__(self, type, expr):
        # type: (IrType, IrExpression) -> None
        IrValue.__init__(self, type)
        self._expression = expr

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        self._expression.resolve(module, position)
        IrValue.resolve(self, module, position)

    def get_type(self):
        # type: () -> IrType
        return self._expression.get_type(self._type)

    def get_position(self):
        # type: () -> IrPosition
        return self._expression.get_position()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._expression.resolve_type(equivalence, return_type, return_position)

    def __str__(self):
        # type: () -> str
        return '%s %s' % (self._type, self._expression)

    def used_by(self, expression):
        # type: (IrExpression) -> None
        self._expression.used_by(expression)

    def create_generator_value(self, generator, code_context):
        # type: (IrccGenerator, IrCodeGenContext) -> IrccExpression
        return self._expression.create_generator_value(self._type, generator, code_context)


class IrValueVoid(IrValue):
    def __init__(self):
        # type: () -> None
        IrValue.__init__(self, IrTypeVoid())

    def get_type(self):
        # type: () -> IrType
        return IrTypeVoid()

    def __str__(self):
        # type: () -> str
        return 'void'

    def create_generator_value(self, generator, code_context):
        # type: (IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_constant_void()


class IrValueMetadata(IrValue):
    def __init__(self, metadata):
        # type: (IrMetadata) -> None
        IrValue.__init__(self, IrTypeMetadata())
        self._metadata = metadata

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> None
        self._metadata.resolve(module, position)
        IrValue.resolve(self, module, position)

    def __str__(self):
        # type: () -> str
        return 'metadata %s' % str(self._metadata)

    def get_type(self):
        # type: () -> IrType
        return self._type

    def get_position(self):
        # type: () -> IrPosition
        return IrPosition('', 0, 0, 0, '')

    def create_generator_value(self, generator, code_context):
        # type: (IrccGenerator, IrCodeGenContext) -> IrccExpression
        raise NotImplementedError


if TYPE_CHECKING:
    from .ir_type import IrType, IrAddressSpaceInference
    from .ir_expr import IrExpression
    from .ir_module import IrModule
    from .ir_metadata import IrMetadata
    from .ir_code import IrCodeGenContext
    from ..ir_codegen import IrccGenerator, IrccExpression