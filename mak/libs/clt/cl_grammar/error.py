import sys
import traceback
from ..cl_ast import Position
from be_typing import TYPE_CHECKING


def p_error(p):
    # type: (lex.LexToken) -> None
    if p:
        p.lexer.logger.C0011(Position(p.filename, p.lineno, p.lexpos, p.endlexpos, p.lexer.lexdata()), p.value, p.type)
    else:
        sys.stderr.write("error: unexpected EOF\n")


if TYPE_CHECKING:
    from ply import lex