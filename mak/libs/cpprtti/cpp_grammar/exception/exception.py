"""
try-block:
      try compound-statement handler-seq
function-try-block:
      try ctor-initializeropt compound-statement handler-seq     C++0x
handler-seq:
      handler
      handler handler-seq
handler:
      catch ( exception-declaration ) compound-statement
exception-declaration:
      attribute-specifier-seqopt type-specifier-seq declarator     C++0x
      attribute-specifier-seqopt type-specifier-seq abstract-declaratoropt     C++0x
      ...     C++0x
throw-expression:
      throw assignment-expressionopt
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_try_block(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        try-block : KW_TRY compound-statement handler-seq
    """


@cpp98
def p_function_try_block(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        function-try-block : KW_TRY ctor-initializer? compound-statement handler-seq
    """


@cpp98
def p_handler_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        handler-seq : handler
                    | handler handler-seq
    """


@cpp98
def p_handler(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        handler : KW_CATCH LPAREN exception-declaration RPAREN compound-statement
    """


@cpp98
def p_exception_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        exception-declaration : attribute-specifier-seq? type-specifier-seq declarator
                              | attribute-specifier-seq? type-specifier-seq abstract-declarator?
                              | ELLIPSIS
    """


@cpp98
def p_throw_exception(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        throw-expression : KW_THROW assignment-expression?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser