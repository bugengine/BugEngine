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

from be_typing import TYPE_CHECKING


def p_initializer(p):
    # type: (YaccProduction) -> None
    """
        initializer : brace-or-equal-initializer
                    | LPAREN expression-list RPAREN
    """


def p_initializer_opt(p):
    # type: (YaccProduction) -> None
    """
        initializer-opt : initializer
                        | empty
    """


def p_brace_or_equal_initializer(p):
    # type: (YaccProduction) -> None
    """
        brace-or-equal-initializer : OP_EQUALS initializer-clause
                                   | braced-init-list
    """


def p_brace_or_equal_initializer_opt(p):
    # type: (YaccProduction) -> None
    """
        brace-or-equal-initializer-opt : brace-or-equal-initializer
                                       | empty
    """


def p_initializer_clause(p):
    # type: (YaccProduction) -> None
    """
        initializer-clause : assignment-expression
                           | braced-init-list
    """


def p_initializer_list(p):
    # type: (YaccProduction) -> None
    """
        initializer-list : initializer-clause ellipsis-opt
                         | initializer-list COMMA initializer-clause ellipsis-opt
    """


def p_braced_init_list(p):
    # type: (YaccProduction) -> None
    """
        braced-init-list : LBRACE initializer-list comma-opt RBRACE
                         | LBRACE RBRACE
    """


def p_braced_init_list_opt(p):
    # type: (YaccProduction) -> None
    """
        braced-init-list-opt : braced-init-list
                             | empty
    """


def p_comma_opt(p):
    # type: (YaccProduction) -> None
    """
        comma-opt : COMMA
                  | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction