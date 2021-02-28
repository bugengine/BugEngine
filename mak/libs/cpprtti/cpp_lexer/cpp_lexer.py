from ply import lex
from ..cpp_position import CppPosition
from be_typing import TYPE_CHECKING
from copy import copy

_LANGLE_OPERATOR_PREDECESSORS = (
    'KW_NEW',
    'KW_DELETE',
    'OP_PLUS',
    'OP_MINUS',
    'OP_TIMES',
    'OP_DIVIDE',
    'OP_MOD',
    'OP_XOR',
    'OP_AND',
    'OP_OR',
    'OP_NOT',
    'OP_LNOT',
    'OP_EQUALS',
    'OP_LT',
    'OP_GT',
    'OP_PLUSEQUAL',
    'OP_MINUSEQUAL',
    'OP_TIMESEQUAL',
    'OP_DIVEQUAL',
    'OP_MODEQUAL',
    'OP_XOREQUAL',
    'OP_ANDEQUAL',
    'OP_OREQUAL',
    'OP_LSHIFT',
    'OP_RSHIFT',
    'OP_LSHIFTEQUAL',
    'OP_RSHIFTEQUAL',
    'OP_EQ',
    'OP_NE',
    'OP_LE',
    'OP_GE',
    'OP_LAND',
    'OP_LOR',
    'OP_PLUSPLUS',
    'OP_MINUSMINUS',
    'COMMA',
    'OP_ARROWMEMBER',
    'OP_ARROW',
)

_ANGLE_BRACKET_INVALID_TOKENS = ('RPAREN', 'RBRACE', 'RBRACKET', 'SEMICOLON')


class CppLexer:
    tokens = ()    # type: Tuple[str,...]
    keywords = ()  # type: Tuple[str,...]

    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(module=self)
        self._lexer.cpp_lexer = self
        self._filename = filename
        self._lexdata = ''
        self._lineno = 0
        self.logger = logger
        self._langle_stack = [0]
        self._lookahead = []       # type: List[lex.LexToken]
        self._pred3_token = None   # type: Optional[lex.LexToken]
        self._pred2_token = None   # type: Optional[lex.LexToken]
        self._pred1_token = None   # type: Optional[lex.LexToken]

    def input(self, text):
        # type: (str) -> None
        self._lexer.input(text)
        self._lexdata = self._lexer.lexdata

    def token(self):
        # type: () -> Optional[lex.LexToken]
        if self._lookahead:
            new_token = self._lookahead.pop(0) # type: Optional[lex.LexToken]
        else:
            new_token = self._lexer.token()

        if new_token is not None:
            new_token.cpp_lexer = self
            new_token.cpp_position = CppPosition(
                self._filename, self._lineno, new_token.lexpos, new_token.lexpos + len(new_token.value), self._lexdata
            )

            if new_token.type in ('LPAREN', 'LBRACE', 'LBRACKET'):
                self.begin_scope()
            elif new_token.type in ('RPAREN', 'RBRACE', 'RBRACKET'):
                self.end_scope()
            elif new_token.type == 'OP_LT':
                if self.scan_rangle(0, self._langle_stack[-1], self._pred1_token, self._pred2_token, self._pred3_token):
                    new_token.type = 'LANGLE'
                    self._langle_stack[-1] += 1
            elif new_token.type == 'OP_GT':
                if self._langle_stack[-1] > 0:
                    self._langle_stack[-1] -= 1
                    new_token.type = 'RANGLE'

        self._pred3_token = self._pred2_token
        self._pred2_token = self._pred1_token
        self._pred1_token = new_token
        return new_token

    def position(self, token):
        # type: (lex.LexToken) -> CppPosition
        return CppPosition(self._filename, self._lineno, token.lexpos, token.lexpos + len(token.value), self._lexdata)

    def eof_position(self):
        # type: () -> CppPosition
        return CppPosition(self._filename, self._lineno, len(self._lexdata), len(self._lexdata), self._lexdata)

    def look_ahead(self, index):
        # type: (int) -> Optional[lex.LexToken]
        while index >= len(self._lookahead):
            token = self._lexer.token()
            if token is None:
                return None
            self._lookahead.append(token)
        return self._lookahead[index]

    def begin_scope(self):
        # type: () -> None
        self._langle_stack.append(0)

    def end_scope(self):
        # type: () -> None
        if len(self._langle_stack) > 1:
            assert self._langle_stack[-1] == 0
            self._langle_stack.pop(-1)

    def scan_rangle(self, start_index, langle_count, pred1_token, pred2_token, pred3_token):
        # type: (int, int, Optional[lex.LexToken], Optional[lex.LexToken], Optional[lex.LexToken]) -> Optional[int]

        def _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token):
            # type: (int, int, Optional[lex.LexToken], Optional[lex.LexToken], Optional[lex.LexToken]) -> Optional[int]
            next_token = self.look_ahead(start_index)
            while next_token is not None:
                if next_token.type == 'OP_LT':
                    next_scan = self.scan_rangle(
                        start_index + 1, langle_count + 1, pred1_token, pred2_token, pred3_token
                    )
                    if next_scan is not None:
                        next_token.type = 'LANGLE'
                        start_index = next_scan
                elif next_token.type == 'RANGLE':
                    # if the scan has broken down a RSHIFT
                    return start_index
                elif next_token.type == 'OP_GT':
                    next_token.type = 'RANGLE'
                    return start_index
                elif next_token.type == 'OP_RSHIFT' and langle_count > 0:
                    # break down the RSHIFT, close one angle bracket, return to close the second one
                    next_token.type = 'RANGLE'
                    self._lookahead.insert(start_index, next_token)
                    return start_index
                elif next_token.type == 'LPAREN':
                    start_index = self.find_next(start_index + 1, 'RPAREN')
                    pred2_token = self.look_ahead(start_index - 2)
                    pred1_token = self.look_ahead(start_index - 1)
                    next_token = self.look_ahead(start_index)
                elif next_token.type == 'LBRACKET':
                    start_index = self.find_next(start_index + 1, 'RBRACKET')
                    pred2_token = self.look_ahead(start_index - 2)
                    pred1_token = self.look_ahead(start_index - 1)
                    next_token = self.look_ahead(start_index)
                elif next_token.type == 'LBRACE':
                    start_index = self.find_next(start_index + 1, 'RBRACE')
                    pred2_token = self.look_ahead(start_index - 2)
                    pred1_token = self.look_ahead(start_index - 1)
                    next_token = self.look_ahead(start_index)
                elif next_token.type in _ANGLE_BRACKET_INVALID_TOKENS:
                    return None
                start_index += 1
                pred3_token = pred2_token
                pred2_token = pred1_token
                pred1_token = next_token
                next_token = self.look_ahead(start_index)
            return None

        if pred1_token is not None:
            if pred1_token.type == 'IDENTIFIER':
                return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token.type == 'KW_TEMPLATE':
                return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token.type in ('RBRACKET', 'RPAREN'):
                if getattr(pred3_token, 'type', '') == 'KW_OPERATOR':
                    return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token.type in _LANGLE_OPERATOR_PREDECESSORS:
                if getattr(pred2_token, 'type', '') == 'KW_OPERATOR':
                    return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
        return None

    def find_next(self, start_index, token_type):
        # type: (int, str) -> int
        next_token = self.look_ahead(start_index)
        while next_token is not None and next_token.type != token_type:
            if next_token.type == 'LPAREN':
                start_index = self.find_next(start_index + 1, 'RPAREN')
            if next_token.type == 'LBRACKET':
                start_index = self.find_next(start_index + 1, 'RBRACKET')
            if next_token.type == 'LBRACE':
                start_index = self.find_next(start_index + 1, 'RBRACE')
            start_index += 1
        return start_index


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from types import ModuleType
    from ..cpp_messages import Logger