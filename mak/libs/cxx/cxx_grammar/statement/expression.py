"""
expression-statement: 	 
      expressionopt ;
"""

from ...cxx_parser import cxx98, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("expression-statement : expression ';'")
@cxx98
def p_expression_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@disabled
def p_expression_statement_disabled(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        expression-statement : SEMI
    """
    # covered by empty declaration


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser