class Specifier:
    def __init__(self, lexer, position, specifier):
        # type: (ClLexer, Position, str) -> None
        self.lexer = lexer
        self.position = position
        self.specifier = specifier

    def __str__(self):
        # type: () -> str
        return self.specifier


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Optional
    from .cppobject import CppObject
    from ..cl_lexer import ClLexer
    from .position import Position