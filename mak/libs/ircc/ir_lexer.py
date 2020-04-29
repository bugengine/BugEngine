from ply import lex
from .ir_position import IrPosition
from . import ir_grammar
from be_typing import TYPE_CHECKING


class IrLexer:
    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(module=ir_grammar.lex)
        self._lexer.ir_lexer = self
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
            new_token.ir_lexer = self
            new_token.position = IrPosition(
                self._filename, self._lineno, new_token.lexpos, new_token.lexpos + len(new_token.value), self._lexdata
            )
        return new_token


if TYPE_CHECKING:
    from typing import Optional
    from .ir_messages import Logger
