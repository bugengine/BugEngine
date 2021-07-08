"""
postfix-expression:
    primary-expression
    postfix-expression [ expr-or-braced-init-list ]
    postfix-expression ( expression-list? )
    simple-type-specifier ( expression-list? )
    typename-specifier ( expression-list? )
    simple-type-specifier braced-init-list
    typename-specifier braced-init-list
    postfix-expression . template? id-expression
    postfix-expression -> template? id-expression
    postfix-expression ++
    postfix-expression --
    dynamic_­cast < type-id > ( expression )
    static_­cast < type-id > ( expression )
    reinterpret_­cast < type-id > ( expression )
    const_­cast < type-id > ( expression )
    typeid ( expression )
    typeid ( type-id )

expression-list:
    initializer-list
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('postfix-expression : primary-expression')
@glrp.rule('postfix-expression : postfix-expression "[" expr-or-braced-init-list "]"')
@glrp.rule('postfix-expression : postfix-expression "(" expression-list? ")"')
@glrp.rule('postfix-expression : simple-type-specifier "(" expression-list? ")"')
@glrp.rule('postfix-expression : typename-specifier "(" expression-list? ")"')
@glrp.rule('postfix-expression : simple-type-specifier braced-init-list')
@glrp.rule('postfix-expression : typename-specifier braced-init-list')
@glrp.rule('postfix-expression : postfix-expression "." template? id-expression')
@glrp.rule('postfix-expression : postfix-expression "->" template? id-expression')
@glrp.rule('postfix-expression : postfix-expression "++"')
@glrp.rule('postfix-expression : postfix-expression "--"')
@glrp.rule('postfix-expression : "dynamic_­cast" "<" type-id ">" "(" expression ")"')
@glrp.rule('postfix-expression : "static_­cast" "<" type-id ">" "(" expression ")"')
@glrp.rule('postfix-expression : "reinterpret_­cast" "<" type-id ">" "(" expression ")"')
@glrp.rule('postfix-expression : "const_­cast" "<" type-id ">" "(" expression ")"')
@glrp.rule('postfix-expression : typeid "(" expression ")"')
@glrp.rule('postfix-expression : typeid "(" type-id ")"')
@cxx98
def postfix_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('expression-list : initializer-list')
@cxx98
def expression_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser