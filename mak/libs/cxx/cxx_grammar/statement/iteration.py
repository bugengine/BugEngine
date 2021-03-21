"""
iteration-statement:
      while ( condition ) statement
      do statement while ( expression ) ;
      for ( for-init-statement conditionopt ; expressionopt ) statement
      for ( for-range-declaration : for-range-initializer ) statement     C++0x
for-init-statement:
      expression-statement
      simple-declaration
for-range-declaration:
      attribute-specifier-seqopt type-specifier-seq declarator     C++0x
for-range-initializer:
      expression braced-init-list     C++0x
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("iteration-statement : 'while' '(' condition ')' statement")
@glrp.rule("iteration-statement : 'do' statement 'while' '(' expression ')' ';'")
@glrp.rule("iteration-statement : 'for' '(' for-init-statement condition? ';' expression? ')' statement")
@cxx98
def p_iteration_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("iteration-statement : 'for' '(' for-range-declaration ':' for-range-initializer ')' statement")
@cxx11
def p_iteration_statement_range(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("for-init-statement : expression-statement | simple-declaration")
@cxx98
def p_for_init_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("for-range-declaration : attribute-specifier-seq? type-specifier-seq declarator")
@cxx11
def p_for_range_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("for-range-initializer : expression braced-init-list")
@cxx11
def p_for_range_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser