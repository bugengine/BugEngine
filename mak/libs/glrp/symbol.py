from be_typing import TYPE_CHECKING


class Symbol:
    def __init__(self, id, name, position):
        # type: (int, str, Position) -> None
        self._id = id
        self._name = name
        self._position = position
        self.value = None  # type: Any

    def name(self):
        # type: () -> str
        return self._name

    def run(self):
        # type: () -> None
        pass


if TYPE_CHECKING:
    from be_typing import Any
    from .position import Position