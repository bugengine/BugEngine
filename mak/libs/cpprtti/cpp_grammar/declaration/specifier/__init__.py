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

from ....cpp_parser import cpp98, cpp11, cpp20
from . import storage_class
from . import function

from be_typing import TYPE_CHECKING


@cpp98
def p_decl_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        decl-specifier : storage-class-specifier
                       | type-specifier
                       | function-specifier
                       | KW_FRIEND
                       | KW_TYPEDEF
    """


@cpp11
def p_decl_specifier_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        decl-specifier : KW_CONSTEXPR
    """


@cpp20
def p_decl_specifier_cpp20(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        decl-specifier : KW_CONSTEVAL
                       | KW_CONSTINIT
    """


@cpp98
def p_decl_specifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        decl-specifier-seq : decl-specifier attribute-specifier-seq?
                           | decl-specifier decl-specifier-seq
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser