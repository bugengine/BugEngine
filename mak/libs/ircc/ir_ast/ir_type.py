from be_typing import TYPE_CHECKING


class IrType:
    pass


class IrTypeReference(IrType):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference


class IrBuiltin(IrType):
    def __init__(self, builtin):
        # type: (str) -> None
        self._builtin = builtin


if TYPE_CHECKING:
    from .ir_reference import IrReference