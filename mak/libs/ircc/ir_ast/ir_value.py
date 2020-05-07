from be_typing import TYPE_CHECKING


class IrValue:
    def __init__(self, type):
        # type: (IrType) -> None
        self._type = type

    def resolve(self, module):
        # type: (IrModule) -> IrValue
        self._type = self._type.resolve(module)
        return self


class IrValueInteger(IrValue):
    def __init__(self, type, value):
        # type: (IrType, int) -> None
        IrValue.__init__(self, type)
        self.value = value


class IrValueString(IrValue):
    def __init__(self, type, value):
        # type: (IrType, str) -> None
        IrValue.__init__(self, type)
        self.value = value


if TYPE_CHECKING:
    from .ir_type import IrType
    from .ir_module import IrModule