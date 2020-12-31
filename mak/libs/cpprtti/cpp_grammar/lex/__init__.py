from .charset import *
from .pptoken import *
from .token import *
from .header import *
from .ppnumber import *
from .name import *
from .keyword import *
from .operators import *
from .literal import *

from ply.lex import TOKEN
from be_typing import TYPE_CHECKING

t_ignore = ' \t'


def t_NEWLINE(t):
    # type: (LexToken) -> Optional[LexToken]
    r'\n+'
    t.lexer.cpp_lexer._lineno += t.value.count("\n")
    return None


@TOKEN(identifier)
def t_IDENTIFIER(t):
    # type: (LexToken) -> Optional[LexToken]
    if t.value in keywords:
        t.type = 'KW_' + t.value.upper()
    return t


def t_error(t):
    # type: (LexToken) -> Optional[LexToken]
    t.lexer.skip(1)
    return None


if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken