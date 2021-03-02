"""
statement:
      labeled-statement
      attribute-specifier-seqopt expression-statement     C++0x
      attribute-specifier-seqopt compound-statement     C++0x
      attribute-specifier-seqopt selection-statement     C++0x
      attribute-specifier-seqopt iteration-statement     C++0x
      attribute-specifier-seqopt jump-statement     C++0x
      declaration-statement
      attribute-specifier-seqopt try-block
"""

from . import label
from . import expression
from . import block
from . import select
from . import iteration
from . import jump
from . import declaration

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        statement : labeled-statement
                  | expression-statement
                  | attribute-specifier-seq? compound-statement
                  | attribute-specifier-seq? selection-statement
                  | attribute-specifier-seq? iteration-statement
                  | attribute-specifier-seq? jump-statement
                  | declaration-statement
                  | attribute-specifier-seq? try-block
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser