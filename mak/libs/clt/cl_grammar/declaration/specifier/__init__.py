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

CHANGE: decl-specifier-seq 
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
        decl-specifier-seq : decl-specifier
                           | decl-specifier attribute-specifier-seq
                           | decl-specifier decl-specifier-seq
                           | type-specifier-seq                         %prec OP_SCOPE
                           | type-specifier-seq decl-specifier-seq      %prec SCOPE_REDUCTION
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction