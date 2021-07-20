"""
(6.5.1) primary-expression:
    identifier
    constant
    string-literal
    ( expression )
    generic-selection
(6.5.1.1) generic-selection:
    _Generic ( assignment-expression , generic-assoc-list )
(6.5.1.1) generic-assoc-list:
    generic-association
    generic-assoc-list , generic-association
(6.5.1.1) generic-association:
    type-name : assignment-expression
    default : assignment-expression
(6.5.2) postfix-expression:
    primary-expression
    postfix-expression [ expression ]
    postfix-expression ( argument-expression-listopt )
    postfix-expression . identifier
    postfix-expression -> identifier
    postfix-expression ++
    postfix-expression --
    ( type-name ) { initializer-list }
    ( type-name ) { initializer-list , }
(6.5.2) argument-expression-list:
    assignment-expression
    argument-expression-list , assignment-expression
(6.5.3) unary-expression:
    postfix-expression
    ++ unary-expression
    -- unary-expression
    unary-operator cast-expression
    sizeof unary-expression
    sizeof ( type-name )
    _Alignof ( type-name )
(6.5.3) unary-operator: one of
    & * + - ~ !
(6.5.4) cast-expression:
    unary-expression
    ( type-name ) cast-expression
(6.5.5) multiplicative-expression:
    cast-expression
    multiplicative-expression * cast-expression
    multiplicative-expression / cast-expression
    multiplicative-expression % cast-expression
(6.5.6) additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression - multiplicative-expression
(6.5.7) shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression
(6.5.8) relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression >= shift-expression
(6.5.9) equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
(6.5.10) AND-expression:
    equality-expression
    AND-expression & equality-expression
(6.5.11) exclusive-OR-expression:
    AND-expression
    exclusive-OR-expression ^ AND-expression
(6.5.12) inclusive-OR-expression:
    exclusive-OR-expression
    inclusive-OR-expression | exclusive-OR-expression
(6.5.13) logical-AND-expression:
    inclusive-OR-expression
    logical-AND-expression && inclusive-OR-expression
(6.5.14) logical-OR-expression:
    logical-AND-expression
    logical-OR-expression || logical-AND-expression
(6.5.15) conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression
(6.5.16) assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression
(6.5.16) assignment-operator: one of
    = *= /= %= += -= <<= >>= &= ^= |=
(6.5.17) expression:
    assignment-expression
    expression , assignment-expression
(6.6) constant-expression:
    conditional-expression
"""

import glrp
from ..parser import c89
from be_typing import TYPE_CHECKING


@c89
@glrp.rule('primary-expression[split] : "identifier"')
@glrp.rule('primary-expression : "integer-constant"')
@glrp.rule('primary-expression : "floating-constant"')
@glrp.rule('primary-expression : "character-constant"')
@glrp.rule('primary-expression : "enumeration-constant"')
@glrp.rule('primary-expression : "string-literal"')
@glrp.rule('primary-expression : [split]"(" expression ")"')
@glrp.rule('primary-expression : generic-selection')
def primary_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('generic-selection : "_Generic" "(" assignment-expression "," generic-assoc-list ")"')
def generic_selection(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('generic-assoc-list : generic-association')
@glrp.rule('generic-assoc-list : generic-assoc-list "," generic-association')
def generic_assoc_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('generic-association : type-name ":" assignment-expression')
@glrp.rule('generic-association : "default" ":" assignment-expression')
def generic_association(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('postfix-expression : primary-expression')
@glrp.rule('postfix-expression : postfix-expression "[" expression "]"')
@glrp.rule('postfix-expression : postfix-expression "(" argument-expression-list? ")"')
@glrp.rule('postfix-expression : postfix-expression "." identifier')
@glrp.rule('postfix-expression : postfix-expression "->" identifier')
@glrp.rule('postfix-expression : postfix-expression "++"')
@glrp.rule('postfix-expression : postfix-expression "--"')
@glrp.rule('postfix-expression : "(" type-name ")" "{" initializer-list "}"')
@glrp.rule('postfix-expression : "(" type-name ")" "{" initializer-list "," "}"')
def postfix_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('argument-expression-list : assignment-expression')
@glrp.rule('argument-expression-list : argument-expression-list "," assignment-expression')
def argument_expression_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('unary-expression : postfix-expression')
@glrp.rule('unary-expression : "++" unary-expression')
@glrp.rule('unary-expression : "--" unary-expression')
@glrp.rule('unary-expression : unary-operator cast-expression')
@glrp.rule('unary-expression : "sizeof" unary-expression')
@glrp.rule('unary-expression : "sizeof" "(" type-name ")"')
@glrp.rule('unary-expression : "_Alignof" "(" type-name ")"')
def unary_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('unary-operator : "&" | "*" | "+" | "-" | "~" | "!"')
def unary_operator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('cast-expression : unary-expression')
@glrp.rule('cast-expression : "(" type-name ")" cast-expression')
def cast_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('multiplicative-expression : cast-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "*" cast-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "/" cast-expression')
@glrp.rule('multiplicative-expression : multiplicative-expression "%" cast-expression')
def multiplicative_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('additive-expression : multiplicative-expression')
@glrp.rule('additive-expression : additive-expression "+" multiplicative-expression')
@glrp.rule('additive-expression : additive-expression "-" multiplicative-expression')
def additive_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('shift-expression : additive-expression')
@glrp.rule('shift-expression : shift-expression "<<" additive-expression')
@glrp.rule('shift-expression : shift-expression ">>" additive-expression')
def shift_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('relational-expression : shift-expression')
@glrp.rule('relational-expression : relational-expression "<" shift-expression')
@glrp.rule('relational-expression : relational-expression ">" shift-expression')
@glrp.rule('relational-expression : relational-expression "<=" shift-expression')
@glrp.rule('relational-expression : relational-expression ">=" shift-expression')
def relational_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('equality-expression : relational-expression')
@glrp.rule('equality-expression : equality-expression "==" relational-expression')
@glrp.rule('equality-expression : equality-expression "!=" relational-expression')
def equality_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('AND-expression : equality-expression')
@glrp.rule('AND-expression : AND-expression "&" equality-expression')
def AND_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('exclusive-OR-expression : AND-expression')
@glrp.rule('exclusive-OR-expression : exclusive-OR-expression "^" AND-expression')
@glrp.rule('inclusive-OR-expression : exclusive-OR-expression')
@glrp.rule('inclusive-OR-expression : inclusive-OR-expression "|" exclusive-OR-expression')
def inclusive_OR_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('logical-AND-expression : inclusive-OR-expression')
@glrp.rule('logical-AND-expression : logical-AND-expression "&&" inclusive-OR-expression')
def logical_AND_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('logical-OR-expression : logical-AND-expression')
@glrp.rule('logical-OR-expression : logical-OR-expression "||" logical-AND-expression')
def logical_OR_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('conditional-expression : logical-OR-expression')
@glrp.rule('conditional-expression : logical-OR-expression "?" expression ":" conditional-expression')
def conditional_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('assignment-expression : conditional-expression')
@glrp.rule('assignment-expression : unary-expression assignment-operator assignment-expression')
def assignment_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('assignment-operator : "=" | "*=" | "/=" | "%=" | "+=" | "-=" | "<<=" | ">>=" | "&=" | "^=" | "|="')
def assignment_operator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('expression : assignment-expression')
@glrp.rule('expression : expression "," assignment-expression')
def expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('constant-expression : conditional-expression')
def constant_expression(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ..parser import CParser