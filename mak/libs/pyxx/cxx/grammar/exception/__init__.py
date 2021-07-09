"""
try-block:
    try compound-statement handler-seq

function-try-block:
    try ctor-initializer? compound-statement handler-seq

handler-seq:
    handler handler-seq?

handler:
    catch ( exception-declaration ) compound-statement

exception-declaration:
    attribute-specifier-seq? type-specifier-seq declarator
    attribute-specifier-seq? type-specifier-seq abstract-declarator?
    ...
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import specification


@glrp.rule('try-block : "try" compound-statement handler-seq')
@cxx98
def try_block(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('function-try-block : "try" ctor-initializer? compound-statement handler-seq')
@cxx98
def function_try_block(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('handler-seq : handler handler-seq?')
@cxx98
def handler_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('handler : "catch" "(" exception-declaration ")" compound-statement')
@cxx98
def handler(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('exception-declaration : attribute-specifier-seq? type-specifier-seq declarator')
@glrp.rule('exception-declaration : attribute-specifier-seq? type-specifier-seq abstract-declarator?')
@glrp.rule('exception-declaration : "..."')
@cxx98
def exception_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser