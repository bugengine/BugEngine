import glrp
from be_typing import TYPE_CHECKING

_LANGLE_OPERATOR_PREDECESSORS = (
    'new',
    'delete',
    '+',
    '-',
    '*',
    '/',
    '%',
    '^',
    '&',
    '|',
    '!',
    '~',
    '=',
    '<',
    '>',
    '+=',
    '-=',
    '*=',
    '/=',
    '%=',
    '^=',
    '&=',
    '|=',
    '<<',
    '>>',
    '<<=',
    '>>=',
    '==',
    '!=',
    '<=',
    '>=',
    '&&',
    '||',
    '++',
    '--',
    ',',
    '->*',
    '->',
)

_ANGLE_BRACKET_INVALID_TOKENS = (')', '}', ']', ';')


class CxxLexer(glrp.Lexer):
    tokens = ('<[', ']>', 'doxycomment') # type: Tuple[str,...]
    keywords = ()                        # type: Tuple[str,...]

    def __init__(self):
        # type: () -> None
        glrp.Lexer.__init__(self)
        self._token_generator = glrp.Lexer.token(self)
        self._lookahead = []   # type: List[glrp.Token]

    def token(self):
        # type: () -> Generator[glrp.Token, None, None]
        langle_stack = [0]
        pred3_token = None     # type: Optional[glrp.Token]
        pred2_token = None     # type: Optional[glrp.Token]
        pred1_token = None     # type: Optional[glrp.Token]

        while True:
            if self._lookahead:
                new_token = self._lookahead.pop(0)
            else:
                try:
                    new_token = next(self._token_generator)
                except StopIteration:
                    break

            # These rules need to be in token() and not in the individual token functions
            # Otherwise, lookahead would trigger those rules and the lookahead stack will become invalid
            if new_token._name in ('(', '{', '['):
                langle_stack.append(0)
            elif new_token._name in (')', '}', ']'):
                if len(langle_stack) > 1:
                    assert langle_stack[-1] == 0
                    langle_stack.pop(-1)
            elif new_token._name == '<':
                if self.scan_rangle(0, langle_stack[-1], pred1_token, pred2_token, pred3_token):
                    self.set_token_type(new_token, '<[')
                    langle_stack[-1] += 1
            elif new_token._name in ('>', ']>'):
                if langle_stack[-1] > 0:
                    langle_stack[-1] -= 1
                    self.set_token_type(new_token, ']>')

            pred3_token = pred2_token
            pred2_token = pred1_token
            pred1_token = new_token
            yield new_token

    def look_ahead(self, index):
        # type: (int) -> glrp.Token
        while index >= len(self._lookahead):
            token = next(self._token_generator)
            self._lookahead.append(token)
        return self._lookahead[index]

    def scan_rangle(self, start_index, langle_count, pred1_token, pred2_token, pred3_token):
        # type: (int, int, Optional[glrp.Token], Optional[glrp.Token], Optional[glrp.Token]) -> Optional[int]

        def _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token):
            # type: (int, int, Optional[glrp.Token], Optional[glrp.Token], Optional[glrp.Token]) -> Optional[int]
            try:
                while True:
                    next_token = self.look_ahead(start_index)
                    if next_token._name == '<':
                        next_scan = self.scan_rangle(
                            start_index + 1, langle_count + 1, pred1_token, pred2_token, pred3_token
                        )
                        if next_scan is not None:
                            self.set_token_type(next_token, '<[')
                            start_index = next_scan
                    elif next_token._name == ']>':
                        # if the scan has broken down a RSHIFT
                        return start_index
                    elif next_token._name == '>':
                        self.set_token_type(next_token, ']>')
                        return start_index
                    elif next_token._name == '>>' and langle_count > 0:
                        # break down the RSHIFT, close one angle bracket, return to close the second one
                        self.set_token_type(next_token, ']>')
                        self._lookahead.insert(start_index, next_token)
                        return start_index
                    elif next_token._name == '(':
                        start_index = self.find_next(start_index + 1, ')')
                        pred2_token = self.look_ahead(start_index - 2)
                        pred1_token = self.look_ahead(start_index - 1)
                        next_token = self.look_ahead(start_index)
                    elif next_token._name == '[':
                        start_index = self.find_next(start_index + 1, ']')
                        pred2_token = self.look_ahead(start_index - 2)
                        pred1_token = self.look_ahead(start_index - 1)
                        next_token = self.look_ahead(start_index)
                    elif next_token._name == '{':
                        start_index = self.find_next(start_index + 1, '}')
                        pred2_token = self.look_ahead(start_index - 2)
                        pred1_token = self.look_ahead(start_index - 1)
                        next_token = self.look_ahead(start_index)
                    elif next_token._name in _ANGLE_BRACKET_INVALID_TOKENS:
                        return None
                    start_index += 1
                    pred3_token = pred2_token
                    pred2_token = pred1_token
                    pred1_token = next_token
            except StopIteration:
                return None

        if pred1_token is not None:
            if pred1_token._name == 'identifier':
                return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token._name == 'template':
                return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token._name in (']', ')'):
                if getattr(pred3_token, 'type', '') == 'operator':
                    return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
            elif pred1_token._name in _LANGLE_OPERATOR_PREDECESSORS:
                if getattr(pred2_token, 'type', '') == 'operator':
                    return _scan_rangle_impl(start_index, langle_count, pred1_token, pred2_token, pred3_token)
        return None

    def find_next(self, start_index, token_type):
        # type: (int, str) -> int
        next_token = self.look_ahead(start_index)
        while next_token is not None and next_token._name != token_type:
            if next_token._name == '(':
                start_index = self.find_next(start_index + 1, ')')
            if next_token._name == '{':
                start_index = self.find_next(start_index + 1, '}')
            if next_token._name == '[':
                start_index = self.find_next(start_index + 1, ']')
            start_index += 1
        return start_index


if TYPE_CHECKING:
    from typing import Generator, List, Optional, Tuple
    from types import ModuleType
    from ..cxx_messages import Logger