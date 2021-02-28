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

from ....cpp_parser import cpp98
from . import simple
from . import elaborate
from be_typing import TYPE_CHECKING


@cpp98
def p_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-specifier : trailing-type-specifier
                       | class-specifier
                       | enum-specifier
    """


@cpp98
def p_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-specifier : trailing-type-specifier
    """


@cpp98
def p_trailing_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        trailing-type-specifier : simple-type-specifier
                                | elaborated-type-specifier
                                | typename-specifier
    """


@cpp98
def p_trailing_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        trailing-type-specifier : simple-type-specifier
    """


@cpp98
def p_type_specifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-specifier-seq : type-specifier attribute-specifier-seq?
                           | type-specifier type-specifier-seq
    """


@cpp98
def p_trailing_type_specifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        trailing-type-specifier-seq : trailing-type-specifier attribute-specifier-seq?
                                    | trailing-type-specifier trailing-type-specifier-seq
    """


#def p_type_specifier_seq(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        type-specifier-seq : cv-qualifier? type-specifier cv-qualifier? attribute-specifier-seq?
#    """

#def p_trailing_type_specifier_seq(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        trailing-type-specifier-seq : cv-qualifier? trailing-type-specifier cv-qualifier? attribute-specifier-seq?
#qqq    """

if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser