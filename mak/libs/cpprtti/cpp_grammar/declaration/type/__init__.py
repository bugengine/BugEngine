"""
type-specifier:
      trailing-type-specifier
      class-specifier
      enum-specifier
trailing-type-specifier:
      simple-type-specifier
      elaborated-type-specifier
      typename-specifier
      cv-qualifier
type-specifier-seq:
      type-specifier attribute-specifier-seqopt     C++0x
      type-specifier type-specifier-seq
trailing-type-specifier-seq:
      trailing-type-specifier attribute-specifier-seqopt     C++0x
      trailing-type-specifier trailing-type-specifier-seq     C++0x
"""

from .simple import *
from .elaborate import *
from be_typing import TYPE_CHECKING


def p_type_specifier(p):
    # type: (YaccProduction) -> None
    """
        type-specifier : trailing-type-specifier
                       | class-specifier
                       | enum-specifier
    """


def p_trailing_type_specifier(p):
    # type: (YaccProduction) -> None
    """
        trailing-type-specifier : simple-type-specifier
                                | elaborated-type-specifier
                                | typename-specifier
                                | cv-qualifier
    """


def p_type_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        type-specifier-seq : type-specifier attribute-specifier-seq?
                           | type-specifier type-specifier-seq
    """


def p_trailing_type_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        trailing-type-specifier-seq : trailing-type-specifier attribute-specifier-seq?
                                    | trailing-type-specifier trailing-type-specifier-seq
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction