from ply import lex
from .ir_position import IrPosition
from . import ir_grammar
from be_typing import TYPE_CHECKING
from copy import copy


class IrLexer:
    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(module=ir_grammar.lex)
        self._lexer.ir_lexer = self
        self._filename = filename
        self._lexdata = ''
        self._lineno = 1
        self._keywords_enabled = True
        self._last_token = None    # type: Optional[lex.LexToken]
        self.logger = logger

    def input(self, text):
        # type: (str) -> None
        self._lexer.input(text)
        self._lexdata = self._lexer.lexdata

    def token(self):
        # type: () -> Optional[lex.LexToken]
        if self._last_token is None or self._last_token.type != 'LPAREN':
            new_token = self._lexer.token()
        else:
            new_token = copy(self._last_token)
            new_token.type = 'LPAREN_MARK'
        if new_token:
            new_token.ir_lexer = self
            new_token.position = self.position(new_token)
        self._last_token = new_token
        return new_token

    def position(self, token):
        # type: (lex.LexToken) -> IrPosition
        return IrPosition(self._filename, self._lineno, token.lexpos, token.lexpos + len(token.value), self._lexdata)

    def eof_position(self):
        # type: () -> IrPosition
        return IrPosition(self._filename, self._lineno, len(self._lexdata), len(self._lexdata), self._lexdata)

    def enable_keywords(self):
        # type: () -> None
        self._keywords_enabled = True

    def disable_keywords(self):
        # type: () -> None
        self._keywords_enabled = False


if TYPE_CHECKING:
    from typing import Optional
    from .ir_messages import Logger
