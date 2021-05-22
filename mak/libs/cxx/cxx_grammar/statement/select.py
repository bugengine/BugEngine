"""
selection-statement:
      if ( condition ) statement
      if ( condition ) statement else statement
      switch ( condition ) statement
condition:
      expression
      attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause     C++0x
      attribute-specifier-seqopt decl-specifier-seq declarator braced-init-list     C++0x
"""

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("selection-statement : 'if' '(' condition ')' statement [prec:left,0]")
@glrp.rule("selection-statement : 'if' '(' condition ')' statement [prec:left,1] 'else' statement")
@glrp.rule("selection-statement : 'switch' '(' condition ')' statement")
@cxx98
def p_selection_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("condition : expression")
@glrp.rule("condition : attribute-specifier-seq? decl-specifier-seq declarator '=' initializer-clause")
@cxx98
def p_condition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("condition : attribute-specifier-seq? decl-specifier-seq declarator braced-init-list")
@cxx11
def p_condition_braced_init(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser