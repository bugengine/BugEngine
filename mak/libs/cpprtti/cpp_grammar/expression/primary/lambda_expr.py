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

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_lambda_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        lambda-expression : lambda-introducer lambda-declarator? compound-statement
    """


@cpp98
def p_lambda_introducer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        lambda-introducer : LBRACKET lambda-capture? RBRACKET
    """


@cpp98
def p_lambda_capture(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        lambda-capture : capture-default
                       | capture-list
                       | capture-default COMMA capture-list
    """


@cpp98
def p_capture_default(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        capture-default : OP_AND
                        | OP_EQUALS
    """


@cpp98
def p_capture_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        capture-list : capture ELLIPSIS?
                     | capture-list COMMA capture ELLIPSIS?
    """


@cpp98
def p_capture(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        capture : IDENTIFIER
                | OP_AND IDENTIFIER
                | KW_THIS
    """


@cpp98
def p_lambda_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        lambda-declarator : LPAREN parameter-declaration-clause RPAREN KW_MUTABLE? exception-specification? attribute-specifier-seq? trailing-return-type?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser