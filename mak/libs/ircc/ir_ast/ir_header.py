from be_typing import TYPE_CHECKING


class IrHeader:
    def __init__(self, name, value):
        # type: (str, str) -> None
        self._name = name
        self._value = value

    def visit(self, visitor):
        # type: (IrccGenerator) -> None
        visitor.header_specifier(self._name, self._value)


if TYPE_CHECKING:
    from ..ir_codegen import IrccGenerator