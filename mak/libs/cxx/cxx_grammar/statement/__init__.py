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

import glrp
from ...cxx_parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule("statement : labeled-statement")
@glrp.rule("statement : attribute-specifier-seq? expression-statement")
@glrp.rule("statement : attribute-specifier-seq? compound-statement")
@glrp.rule("statement : attribute-specifier-seq? selection-statement")
@glrp.rule("statement : attribute-specifier-seq? iteration-statement")
@glrp.rule("statement : attribute-specifier-seq? jump-statement")
@glrp.rule("statement : declaration-statement")
@glrp.rule("statement : attribute-specifier-seq? try-block")
@cxx98
def p_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser