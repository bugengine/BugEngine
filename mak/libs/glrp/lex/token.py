from ..symbol import Symbol
from be_typing import TYPE_CHECKING


class Token(Symbol):
    def __init__(self, id, type, position, value, skipped_tokens):
        # type: (int, str, Position, Any, List[Token]) -> None
        Symbol.__init__(self, id, type, position)
        self.value = value
        self._skipped_tokens = skipped_tokens

    def text(self):
        # type: () -> str
        return self._position.text()

    def __str__(self):
        # type: () -> str
        return 'Token(%s, %s)' % (self._name, self.text())


if TYPE_CHECKING:
    from be_typing import Any, List
    from ..position import Position
