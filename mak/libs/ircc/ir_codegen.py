from be_typing import TYPE_CHECKING


class IrCodeGenerator:
    def begin_module(self):
        # type: () -> None
        pass

    def end_module(self):
        # type: () -> None
        pass

    def begin_header(self):
        # type: () -> None
        pass

    def end_header(self):
        # type: () -> None
        pass


if TYPE_CHECKING:
    from typing import Any