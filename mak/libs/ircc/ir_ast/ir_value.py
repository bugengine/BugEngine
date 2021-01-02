from .ir_object import IrObject
from .ir_type import IrTypeMetadata, IrTypeBuiltin, IrTypeVoid
from be_typing import TYPE_CHECKING


class IrValue(IrObject):
    def __init__(self, type):
        # type: (IrType) -> None
        IrObject.__init__(self)
        self._type = type

    def resolve(self, module):
        # type: (IrModule) -> IrValue
        if self._type is not None:
            self._type = self._type.resolve(module)
        return self

    def get_type(self):
        # type: () -> IrType
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        raise NotImplementedError


class IrValueExpr(IrValue):
    def __init__(self, type, expr):
        # type: (IrType, IrExpression) -> None
        IrValue.__init__(self, type)
        self._expression = expr

    def resolve(self, module):
        # type: (IrModule) -> IrValue
        self._expression = self._expression.resolve(module)
        return IrValue.resolve(self, module)

    def get_type(self):
        # type: () -> IrType
        return self._expression.get_type()

    def __str__(self):
        # type: () -> str
        return '%s %s' % (self._type, self._expression)


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


class IrValueMetadata(IrValue):
    def __init__(self, metadata):
        # type: (IrMetadata) -> None
        IrValue.__init__(self, IrTypeMetadata())
        self._metadata = metadata

    def resolve(self, module):
        # type: (IrModule) -> IrValue
        self._metadata = self._metadata.resolve(module)
        return IrValue.resolve(self, module)

    def __str__(self):
        # type: () -> str
        return 'metadata %s' % str(self._metadata)


if TYPE_CHECKING:
    from .ir_type import IrType
    from .ir_expr import IrExpression
    from .ir_module import IrModule
    from .ir_metadata import IrMetadata