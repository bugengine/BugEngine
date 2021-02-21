"""
class-name:
      identifier
      simple-template-id     C++0x
class-specifier:
      class-head { member-specificationopt }
class-head:
      class-key attribute-specifier-seqopt class-head-name class-virt-specifier-seqopt base-clauseopt     C++0x
      class-key attribute-specifier-seqopt base-clauseopt     C++0x
class-head-name:
      nested-name-specifieropt class-name     C++0x
class-virt-specifier-seq:
      class-virt-specifier     C++0x
      class-virt-specifier-seq class-virt-specifier     C++0x
class-virt-specifier:
      final     C++0x
      explicit     C++0x
class-key:
      class
      struct
      union
"""

from .member import *
from .base import *
from .conversion import *
from .base_init import *

from be_typing import TYPE_CHECKING


def p_class_name(p):
    # type: (YaccProduction) -> None
    """
        class-name : IDENTIFIER
                   | simple-template-id
    """


def p_class_specifier(p):
    # type: (YaccProduction) -> None
    """
        class-specifier : class-head LBRACE member-specification? RBRACE
    """


def p_class_head(p):
    # type: (YaccProduction) -> None
    """
        class-head : class-key attribute-specifier-seq? class-head-name class-virt-specifier-seq? base-clause?
                   | class-key attribute-specifier-seq? base-clause?
    """


def p_class_head_name(p):
    # type: (YaccProduction) -> None
    """
        class-head-name : nested-name-specifier? class-name
    """


def p_class_virt_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        class-virt-specifier-seq : class-virt-specifier
                                 | class-virt-specifier-seq class-virt-specifier
    """


def p_class_virt_specifier(p):
    # type: (YaccProduction) -> None
    """
        class-virt-specifier : KW_FINAL
                             | KW_EXPLICIT
    """


def p_class_key(p):
    # type: (YaccProduction) -> None
    """
        class-key : KW_CLASS
                  | KW_STRUCT
                  | KW_UNION
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction