from be_typing import TYPE_CHECKING


class IrCodeGenerator:
    def begin_module(self):
        # type: () -> None
        pass

    def end_module(self):
        # type: () -> None
        pass

    def begin_headers(self):
        # type: () -> None
        pass

    def header_specifier(self, name, value):
        # type: (str, str) -> None
        pass

    def end_headers(self):
        # type: () -> None
        pass

    def begin_declarations(self):
        # type: () -> None
        pass

    def end_declarations(self):
        # type: () -> None
        pass


if TYPE_CHECKING:
    from typing import Any