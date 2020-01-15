import sys
import traceback
from ..cl_ast import Position
from be_typing import TYPE_CHECKING


def p_error(p):
    # type: (lex.LexToken) -> None
    if p:
        p.lexer.error(
            "syntax error near token %s (%s)" % (p.value, p.type),
            Position(p.filename, p.lineno, p.lexpos, p.endlexpos)
        )
    else:
        sys.stderr.write("error: unexpected EOF\n")


if TYPE_CHECKING:
    from ply import lex