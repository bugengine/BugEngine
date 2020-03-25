"""
expression:
      assignment-expression
      expression , assignment-expression
"""

from .primary import *
from .postfix import *
from .unary import *
from .new import *
from .delete import *
from .unary_noexcept import *
from .cast import *
from .member_pointer import *
from .multiplicative import *
from .additive import *
from .shift import *
from .relational import *
from .equality import *
from .bit_and import *
from .exclusive_or import *
from .inclusive_or import *
from .logical_and import *
from .logical_or import *
from .conditional import *
from .assignment import *
from .comma import *
from .constant import *

from be_typing import TYPE_CHECKING


def p_expression(p):
    # type: (YaccProduction) -> None
    """
        expression : assignment-expression
                   | expression COMMA assignment-expression
    """


def p_expression_opt(p):
    # type: (YaccProduction) -> None
    """
        expression-opt : expression
    """
    # TODO: empty


if TYPE_CHECKING:
    from ply.yacc import YaccProduction