"""
initializer:
      brace-or-equal-initializer     C++0x
      ( expression-list )     C++0x
brace-or-equal-initializer:
      = initializer-clause     C++0x
      braced-init-list     C++0x
initializer-clause:
      assignment-expression     C++0x
      braced-init-list     C++0x
initializer-list:
      initializer-clause ...opt     C++0x
      initializer-list , initializer-clause ...opt     C++0x
braced-init-list:
      { initializer-list ,opt }     C++0x
      { }     C++0x
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        initializer : brace-or-equal-initializer
                    | LPAREN expression-list RPAREN
    """


@cpp98
def p_brace_or_equal_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        brace-or-equal-initializer : OP_EQUALS initializer-clause
    """


@cpp11
def p_brace_or_equal_initializer_braced_init_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        brace-or-equal-initializer : braced-init-list
    """


@cpp98
def p_initializer_clause(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        initializer-clause : assignment-expression
    """


@cpp11
def p_initializer_clause_braced_init_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        initializer-clause : braced-init-list
    """


@cpp98
def p_initializer_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        initializer-list : initializer-clause ELLIPSIS?
                         | initializer-list COMMA initializer-clause ELLIPSIS?
    """


@cpp98
def p_braced_init_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        braced-init-list : LBRACE initializer-list COMMA? RBRACE
                         | LBRACE RBRACE
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser