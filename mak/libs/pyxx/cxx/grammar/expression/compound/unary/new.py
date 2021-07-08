"""
new-expression:
    ::? new new-placement? new-type-id new-initializer?
    ::? new new-placement? ( type-id ) new-initializer?

new-placement:
    ( expression-list )

new-type-id:
    type-specifier-seq new-declarator?

new-declarator:
    ptr-operator new-declarator?
    noptr-new-declarator

noptr-new-declarator:
    [ expression? ] attribute-specifier-seq?
    noptr-new-declarator [ constant-expression ] attribute-specifier-seq?

new-initializer:
    ( expression-list? )
    braced-init-list
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('new-expression : "::"? "new" new-placement? new-type-id new-initializer?')
@glrp.rule('new-expression : "::"? "new" new-placement? "(" type-id ")" new-initializer?')
@cxx98
def new_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('new-placement : "(" expression-list ")"')
@cxx98
def new_placement(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('new-type-id : type-specifier-seq new-declarator?')
@cxx98
def new_type_id(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('new-declarator : ptr-operator new-declarator?')
@glrp.rule('new-declarator : noptr-new-declarator')
@cxx98
def new_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('noptr-new-declarator : "[" expression? "]" attribute-specifier-seq?')
@glrp.rule('noptr-new-declarator : noptr-new-declarator "[" constant-expression "]" attribute-specifier-seq?')
@cxx98
def noptr_new_declarator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('new-initializer : "(" expression-list? ")"')
@glrp.rule('new-initializer : braced-init-list')
@cxx98
def new_initializer(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser