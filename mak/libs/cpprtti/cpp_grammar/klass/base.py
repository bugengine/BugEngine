"""
base-clause:
      : base-specifier-list
base-specifier-list:
      base-specifier ...opt     C++0x
      base-specifier-list , base-specifier ...opt     C++0x
base-specifier:
      attribute-specifier-seqopt base-type-specifier     C++0x
      attribute-specifier-seqopt virtual access-specifieropt base-type-specifier     C++0x
      attribute-specifier-seqopt access-specifier virtualopt base-type-specifier     C++0x
class-or-decltype:
      ::opt nested-name-specifieropt class-name     C++0x
      decltype-specifier     C++0x
base-type-specifier:
      class-or-decltype     C++0x
access-specifier:
      private
      protected
      public
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_base_clause(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        base-clause : COLON base-specifier-list
    """


@cpp98
def p_base_specifier_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        base-specifier-list : base-specifier ELLIPSIS?
                            | base-specifier-list COMMA base-specifier ELLIPSIS?
    """


@cpp98
def p_base_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        base-specifier : attribute-specifier-seq? base-type-specifier
                       | attribute-specifier-seq? KW_VIRTUAL access-specifier? base-type-specifier
                       | attribute-specifier-seq? access-specifier KW_VIRTUAL? base-type-specifier
    """


@cpp98
def p_class_or_decltype(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-or-decltype : id-expression
    """


@cpp11
def p_class_or_decltype_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-or-decltype : decltype-specifier
    """


@cpp98
def p_base_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        base-type-specifier : class-or-decltype
    """


@cpp98
def p_access_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        access-specifier : KW_PRIVATE
                         | KW_PROTECTED
                         | KW_PUBLIC
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser