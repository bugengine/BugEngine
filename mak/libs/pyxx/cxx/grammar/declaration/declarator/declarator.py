"""
declarator:
    ptr-declarator
    noptr-declarator parameters-and-qualifiers trailing-return-type

ptr-declarator:
    noptr-declarator
    ptr-operator ptr-declarator

noptr-declarator:
    declarator-id attribute-specifier-seq?
    noptr-declarator parameters-and-qualifiers
    noptr-declarator [ constant-expression? ] attribute-specifier-seq?
    ( ptr-declarator )

parameters-and-qualifiers:
    ( parameter-declaration-clause ) cv-qualifier-seq? ref-qualifier? noexcept-specifier? attribute-specifier-seq?

trailing-return-type:
    -> type-id

ptr-operator:
    * attribute-specifier-seq? cv-qualifier-seq?
    & attribute-specifier-seq?
    && attribute-specifier-seq?
    nested-name-specifier * attribute-specifier-seq? cv-qualifier-seq?

cv-qualifier-seq:
    cv-qualifier cv-qualifier-seq?

cv-qualifier:
    const
    volatile

ref-qualifier:
    &
    &&

declarator-id:
    ...? id-expression
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('declarator : ptr-declarator')
@glrp.rule('declarator : noptr-declarator parameters-and-qualifiers trailing-return-type')
@cxx98
def declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('ptr-declarator : noptr-declarator')
@glrp.rule('ptr-declarator : ptr-operator ptr-declarator')
@cxx98
def ptr_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('noptr-declarator : declarator-id attribute-specifier-seq?')
@glrp.rule('noptr-declarator : noptr-declarator parameters-and-qualifiers')
@glrp.rule('noptr-declarator : noptr-declarator "[" constant-expression? "]" attribute-specifier-seq?')
@glrp.rule('noptr-declarator : "(" ptr-declarator ")"')
@cxx98
def noptr_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule(
    'parameters-and-qualifiers : "(" parameter-declaration-clause ")" cv-qualifier-seq? ref-qualifier? noexcept-specifier? attribute-specifier-seq?'
)
@cxx98
def parameters_and_qualifiers(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('trailing-return-type : "->" type-id')
@cxx98
def trailing_return_type(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('ptr-operator : "*" attribute-specifier-seq? cv-qualifier-seq?')
@glrp.rule('ptr-operator : "&" attribute-specifier-seq?')
@glrp.rule('ptr-operator : "&&" attribute-specifier-seq?')
@glrp.rule('ptr-operator : nested-name-specifier "*" attribute-specifier-seq? cv-qualifier-seq?')
@cxx98
def ptr_operator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('cv-qualifier-seq : cv-qualifier cv-qualifier-seq?')
@cxx98
def cv_qualifier_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('cv-qualifier : "const"')
@glrp.rule('cv-qualifier : "volatile"')
@cxx98
def cv_qualifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('ref-qualifier : "&"')
@glrp.rule('ref-qualifier : "&&"')
@cxx98
def ref_qualifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('declarator-id : "..."? id-expression')
@cxx98
def declarator_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser