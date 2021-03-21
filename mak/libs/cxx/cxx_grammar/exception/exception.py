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

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("try-block : 'try' compound-statement handler-seq")
@cxx98
def p_try_block(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("function-try-block : 'try' ctor-initializer? compound-statement handler-seq")
@cxx98
def p_function_try_block(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("handler-seq : handler | handler handler-seq")
@cxx98
def p_handler_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("handler : 'catch' '(' exception-declaration ')' compound-statement")
@cxx98
def p_handler(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("exception-declaration : attribute-specifier-seq? type-specifier-seq declarator")
@glrp.rule("exception-declaration : attribute-specifier-seq? type-specifier-seq abstract-declarator?")
@glrp.rule("exception-declaration : '...'")
@cxx98
def p_exception_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("throw-expression : 'throw' assignment-expression?")
@cxx98
def p_throw_exception(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser