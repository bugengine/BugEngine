from .ir_object import IrObject
from .ir_type import IrTypeMetadata, IrTypeBuiltin, IrTypeVoid
from ..ir_position import IrPosition
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

    def get_position(self):
        # type: () -> IrPosition
        raise NotImplementedError

    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        return


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
        if self._expression.is_typed():
            return self._expression.get_type()
        else:
            return self._type

    def get_position(self):
        # type: () -> IrPosition
        return self._expression.get_position()

    def resolve_type(self, equivalence, return_type, return_position):
        # type: (IrAddressSpaceInference, IrType, IrPosition) -> None
        self._expression.resolve_type(equivalence, return_type, return_position)

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

    def get_type(self):
        # type: () -> IrType
        return self._type

    def get_position(self):
        # type: () -> IrPosition
        return IrPosition('', 0, 0, 0, '')


if TYPE_CHECKING:
    from .ir_type import IrType, IrAddressSpaceInference
    from .ir_expr import IrExpression
    from .ir_module import IrModule
    from .ir_metadata import IrMetadata