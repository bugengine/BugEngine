from ply import lex
from .cpp_position import CppPosition
from . import cpp_grammar
from be_typing import TYPE_CHECKING


class CppLexer:
    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(module=cpp_grammar.lex)
        self._lexer.cpp_lexer = self
        self._filename = filename
        self._lexdata = ''
        self._lineno = 0
        self.logger = logger

    def input(self, text):
        # type: (str) -> None
        self._lexer.input(text)
        self._lexdata = self._lexer.lexdata

    def token(self):
        # type: () -> Optional[lex.LexToken]
        new_token = self._lexer.token()
        if new_token:
            new_token.cpp_lexer = self
            new_token.cpp_position = CppPosition(
                self._filename, self._lineno, new_token.lexpos, new_token.lexpos + len(new_token.value), self._lexdata
            )
        return new_token


if TYPE_CHECKING:
    from typing import Optional
    from .cpp_messages import Logger