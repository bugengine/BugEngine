from be_typing import TYPE_CHECKING


def t_comment_block(t):
    # type: (LexToken) -> Optional[LexToken]
    r'/\*(.|\n)*?\*/'
    t.lexer.lineno += t.value.count('\n')
    return None


def t_comment_line(t):
    # type: (LexToken) -> Optional[LexToken]
    r'\//([^\\\n]|(\\.)|(\\\n))*'
    t.lexer.lineno += t.value.count('\n')
    return None


if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken