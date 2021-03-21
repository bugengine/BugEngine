from be_typing import TYPE_CHECKING


class Symbol:
    def __init__(self, id, name, position):
        # type: (int, str, Position) -> None
        self._id = id
        self._name = name
        self._position = position

    def name(self):
        # type: () -> str
        return self._name


if TYPE_CHECKING:
    from .position import Position