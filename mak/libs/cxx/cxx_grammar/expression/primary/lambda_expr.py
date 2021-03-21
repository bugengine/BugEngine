"""
lambda-expression: 	 
      lambda-introducer lambda-declaratoropt compound-statement     C++0x
lambda-introducer: 	 
      [ lambda-captureopt ]     C++0x
lambda-capture: 	 
      capture-default     C++0x
      capture-list     C++0x
      capture-default , capture-list     C++0x
capture-default: 	 
      &     C++0x
      =     C++0x
capture-list: 	 
      capture ...opt     C++0x
      capture-list , capture ...opt     C++0x
capture: 	 
      identifier     C++0x
      & identifier     C++0x
      this     C++0x
lambda-declarator: 	 
      ( parameter-declaration-clause ) mutableopt exception-specificationopt attribute-specifier-seqopt trailing-return-typeopt     C++0x
"""

from ....cxx_parser import cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("lambda-expression : lambda-introducer lambda-declarator? compound-statement")
@cxx11
def p_lambda_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("lambda-introducer : '[' lambda-capture? ']'")
@cxx11
def p_lambda_introducer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("lambda-capture : capture-default")
@glrp.rule("lambda-capture : capture-list")
@glrp.rule("lambda-capture : capture-default ',' capture-list")
@cxx11
def p_lambda_capture(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("capture-default : '&' | '='")
@cxx11
def p_capture_default(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("capture-list : capture '...'?")
@glrp.rule("capture-list : capture-list ',' capture '...'?")
@cxx11
def p_capture_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("capture : 'identifier' | '&' 'identifier' | 'this'")
@cxx11
def p_capture(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule(
    "lambda-declarator : '(' parameter-declaration-clause ')' 'mutable'? exception-specification? attribute-specifier-seq? trailing-return-type?"
)
@cxx11
def p_lambda_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser