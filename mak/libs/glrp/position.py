from be_typing import TYPE_CHECKING


class Position:
    def __init__(self, lexer, start, end, absolute):
        # type: (Lexer, Tuple[int, int], Tuple[int, int], Tuple[int, int]) -> None
        self._lexer = lexer
        self.start = start
        self.end = end
        self._absolute = absolute

    def extend(self, other):
        # type: (Position) -> Position
        start = min(self.start, other.start)
        end = max(self.end, other.end)
        absolute = (min(self._absolute[0], other._absolute[0]), max(self._absolute[0], other._absolute[0]))
        return Position(self._lexer, start, end, absolute)

    def filename(self):
        # type: () -> str
        return self._lexer._filename

    def text(self):
        # type: () -> str
        return self._lexer._lexdata[self._absolute[0]:self._absolute[1]]

    def context(self):
        # type: () -> List[str]
        index_start = self._absolute[0]
        index_end = self._absolute[1]
        while index_start > 0:
            if self._lexer._lexdata[index_start - 1] == '\n':
                break
            index_start -= 1

        while index_end < len(self._lexer._lexdata) - 1:
            if self._lexer._lexdata[index_end + 1] == '\n':
                break
            index_end += 1
        return self._lexer._lexdata[index_start:index_end + 1].split('\n')


if TYPE_CHECKING:
    from typing import List
    from .lex import Lexer
    from typing import Tuple