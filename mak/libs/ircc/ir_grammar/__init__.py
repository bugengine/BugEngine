from . import lex

from .translation_unit import *

from be_typing import TYPE_CHECKING


def p_error(p):
    # type: (YaccProduction) -> None
    """
        error :
    """


def p_empty(p):
    # type: (YaccProduction) -> None
    """
        empty :
    """
    p[0] = None


tokens = lex.tokens

if TYPE_CHECKING:
    from ply.yacc import YaccProduction