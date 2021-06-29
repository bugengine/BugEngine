"""
lambda-expression:
    lambda-introducer lambda-declarator compound-statement
    lambda-introducer < template-parameter-list > requires-clause? lambda-declarator compound-statement

lambda-introducer:
    [ lambda-capture? ]

lambda-declarator:
    lambda-specifiers
    ( parameter-declaration-clause ) lambda-specifiers requires-clause?

lambda-specifiers:
    decl-specifier-seq? noexcept-specifier? attribute-specifier-seq? trailing-return-type?
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('lambda-expression : lambda-introducer lambda-declarator compound-statement')
@glrp.rule(
    'lambda-expression : lambda-introducer "<" template-parameter-list ">" requires-clause? lambda-declarator compound-statement'
)
@cxx98
def lambda_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('lambda-introducer : "[" lambda-capture? "]"')
@cxx98
def lambda_introducer(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('lambda-declarator : lambda-specifiers')
@glrp.rule('lambda-declarator : "(" parameter-declaration-clause ")" lambda-specifiers requires-clause?')
@cxx98
def lambda_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('lambda-specifiers : decl-specifier-seq? noexcept-specifier? attribute-specifier-seq? trailing-return-type?')
@cxx98
def lambda_specifiers(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser