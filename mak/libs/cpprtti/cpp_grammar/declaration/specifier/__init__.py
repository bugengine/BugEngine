"""
decl-specifier:
  	storage-class-specifier
  	type-specifier
  	function-specifier
  	friend
  	typedef
  	constexpr     C++0x
decl-specifier-seq:
  	decl-specifier attribute-specifier-seqopt     C++0x
  	decl-specifier decl-specifier-seq     C++0x
"""

from .storage_class import *
from .function import *

from be_typing import TYPE_CHECKING


def p_decl_specifier(p):
    # type: (YaccProduction) -> None
    """
        decl-specifier : storage-class-specifier
                       | function-specifier
                       | KW_FRIEND
                       | KW_TYPEDEF
                       | KW_CONSTEXPR
    """


def p_decl_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        decl-specifier-seq : decl-specifier attribute-specifier-seq-opt
                           | decl-specifier decl-specifier-seq
    """


def p_decl_specifier_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        decl-specifier-seq-opt : decl-specifier-seq
                               | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction