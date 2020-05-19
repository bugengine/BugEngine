from be_typing import TYPE_CHECKING


class IrObject:
    def __init__(self):
        # type: () -> None
        self._name = None  # type: Optional[str]


if TYPE_CHECKING:
    from typing import Optional