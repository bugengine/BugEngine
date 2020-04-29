
from ply.lex import TOKEN
from be_typing import TYPE_CHECKING

t_ignore = ' \t'


def t_NEWLINE(t):
    # type: (LexToken) -> Optional[LexToken]
    r'\n+'
    t.lexer.ir_lexer._lineno += t.value.count("\n")
    return None


def t_error(t):
    # type: (LexToken) -> Optional[LexToken]
    t.lexer.skip(1)
    return None


tokens = (
    'ID',
)

if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken