"""
type-id:
    type-specifier-seq abstract-declarator?

defining-type-id:
    defining-type-specifier-seq abstract-declarator?

abstract-declarator:
    ptr-abstract-declarator
    noptr-abstract-declarator? parameters-and-qualifiers trailing-return-type
    abstract-pack-declarator

ptr-abstract-declarator:
    noptr-abstract-declarator
    ptr-operator ptr-abstract-declarator?

noptr-abstract-declarator:
    noptr-abstract-declarator? parameters-and-qualifiers
    noptr-abstract-declarator? [ constant-expression? ] attribute-specifier-seq?
    ( ptr-abstract-declarator )

abstract-pack-declarator:
    noptr-abstract-pack-declarator
    ptr-operator abstract-pack-declarator

noptr-abstract-pack-declarator:
    noptr-abstract-pack-declarator parameters-and-qualifiers
    noptr-abstract-pack-declarator [ constant-expression? ] attribute-specifier-seq?
    ...
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


#@glrp.rule('type-id : type-specifier-seq abstract-declarator?')
@glrp.rule('type-id[split] : type-specifier-seq')
@glrp.rule('type-id : type-specifier-seq abstract-declarator')
@cxx98
def type_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('defining-type-id : defining-type-specifier-seq abstract-declarator?')
@cxx98
def defining_type_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('abstract-declarator : ptr-abstract-declarator')
@glrp.rule('abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers trailing-return-type')
@glrp.rule('abstract-declarator : abstract-pack-declarator')
@cxx98
def abstract_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('ptr-abstract-declarator : noptr-abstract-declarator')
@glrp.rule('ptr-abstract-declarator : ptr-operator ptr-abstract-declarator?')
@cxx98
def ptr_abstract_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('noptr-abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers')
@glrp.rule(
    'noptr-abstract-declarator : noptr-abstract-declarator? "[" constant-expression? "]" attribute-specifier-seq?'
)
@glrp.rule('noptr-abstract-declarator : "(" ptr-abstract-declarator ")"')
@cxx98
def noptr_abstract_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('abstract-pack-declarator : noptr-abstract-pack-declarator')
@glrp.rule('abstract-pack-declarator : ptr-operator abstract-pack-declarator')
@cxx98
def abstract_pack_declarato(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('noptr-abstract-pack-declarator : noptr-abstract-pack-declarator parameters-and-qualifiers')
@glrp.rule(
    'noptr-abstract-pack-declarator : noptr-abstract-pack-declarator "[" constant-expression? "]" attribute-specifier-seq?'
)
@glrp.rule('noptr-abstract-pack-declarator : [split]"..."')
@cxx98
def noptr_abstract_pack_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser