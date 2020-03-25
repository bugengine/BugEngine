"""
statement:
      labeled-statement
      attribute-specifier-seqopt expression-statement     C++0x
      attribute-specifier-seqopt compound-statement     C++0x
      attribute-specifier-seqopt selection-statement     C++0x
      attribute-specifier-seqopt iteration-statement     C++0x
      attribute-specifier-seqopt jump-statement     C++0x
      declaration-statement
      attribute-specifier-seqopt try-block
"""

from .label import *
from .expression import *
from .block import *
from .select import *
from .iteration import *
from .jump import *
from .declaration import *

from be_typing import TYPE_CHECKING


def p_statement(p):
    # type: (YaccProduction) -> None
    """
        statement : labeled-statement
                  | attribute-specifier-seq expression-statement
                  | attribute-specifier-seq compound-statement
                  | attribute-specifier-seq selection-statement
                  | attribute-specifier-seq iteration-statement
                  | attribute-specifier-seq jump-statement
                  | expression-statement
                  | compound-statement
                  | selection-statement
                  | iteration-statement
                  | jump-statement
                  | declaration-statement
                  | attribute-specifier-seq try-block
                  | try-block
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction