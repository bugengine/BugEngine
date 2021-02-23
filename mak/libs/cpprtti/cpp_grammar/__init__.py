from . import lex

from .basic import *
from .expression import *
from .statement import *
from .declaration import *
from .namespace import *
from .klass import *
from .overload import *
from .template import *
from .exception import *
from .bugengine_extensions import *

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


precedence = (('left', 'SCOPE_REDUCTION'), ('left', 'OP_SCOPE'))

tokens = lex.tokens

if TYPE_CHECKING:
    from ply.yacc import YaccProduction