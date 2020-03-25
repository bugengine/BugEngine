from ply import lex
from .cl_position import ClPosition
from . import cl_grammar
from be_typing import TYPE_CHECKING


class ClLexer:
    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(module=cl_grammar.lex)
        self._lexer.cl_lexer = self
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
            new_token.cl_lexer = self
            new_token.position = ClPosition(
                self._filename, self._lineno, new_token.lexpos, new_token.lexpos + len(new_token.value), self._lexdata
            )
        return new_token


if TYPE_CHECKING:
    from typing import Optional
    from .cl_messages import Logger