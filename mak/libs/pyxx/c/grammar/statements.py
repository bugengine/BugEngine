"""
(6.8) statement:
    labeled-statement
    unlabeled-statement
(6.8) unlabeled-statement:
    expression-statement
    attribute-specifier-sequenceopt compound-statement
    attribute-specifier-sequenceopt selection-statement
    attribute-specifier-sequenceopt iteration-statement
    attribute-specifier-sequenceopt jump-statement
(6.8.1) label:
    attribute-specifier-sequenceopt identifier :
    attribute-specifier-sequenceopt case constant-expression :
    attribute-specifier-sequenceopt default :
(6.8.1) labeled-statement:
    label statement
(6.8.2) compound-statement:
    { block-item-listopt }
(6.8.2) block-item-list:
    block-item
    block-item-list block-item
(6.8.2) block-item:
    declaration
    unlabeled-statement
    label
(6.8.3) expression-statement:
    expressionopt ;
    attribute-specifier-sequence expression ;
(6.8.4) selection-statement:
    if ( expression ) statement
    if ( expression ) statement else statement
    switch ( expression ) statement
(6.8.5) iteration-statement:
    while ( expression ) statement
    do statement while ( expression ) ;
    for ( expressionopt ; expressionopt ; expressionopt ) statement
    for ( declaration expressionopt ; expressionopt ) statement
(6.8.6) jump-statement:
    goto identifier ;
    continue ;
    break ;
    return expressionopt ;
"""

import glrp
from ..parser import c89
from be_typing import TYPE_CHECKING


@c89
@glrp.rule('statement : labeled-statement')
@glrp.rule('statement : unlabeled-statement')
def statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('unlabeled-statement : expression-statement')
@glrp.rule('unlabeled-statement : attribute-specifier-sequence? compound-statement')
@glrp.rule('unlabeled-statement : attribute-specifier-sequence? selection-statement')
@glrp.rule('unlabeled-statement : attribute-specifier-sequence? iteration-statement')
@glrp.rule('unlabeled-statement : attribute-specifier-sequence? jump-statement')
def unlabeled_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('label : attribute-specifier-sequence? "identifier" ":"')
@glrp.rule('label : attribute-specifier-sequence? "case" constant-expression ":"')
@glrp.rule('label : attribute-specifier-sequence? "default" ":"')
def label(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('labeled-statement : label statement')
def labeled_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('compound-statement : "{" block-item-list? "}"')
def compound_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('block-item-list : block-item')
@glrp.rule('block-item-list : block-item-list block-item')
def block_item_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('block-item : declaration')
@glrp.rule('block-item : unlabeled-statement')
@glrp.rule('block-item : label')
def block_item(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('expression-statement : expression? ";"')
@glrp.rule('expression-statement : attribute-specifier-sequence expression ";"')
def expression_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('selection-statement[prec:left,0] : "if" "(" expression ")" statement')
@glrp.rule('selection-statement : "if" "(" expression ")" statement [prec:left,0]"else" statement')
@glrp.rule('selection-statement : "switch" "(" expression ")" statement')
def selection_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('iteration-statement : "while" "(" expression ")" statement')
@glrp.rule('iteration-statement : "do" statement "while" "(" expression ")" ";"')
@glrp.rule('iteration-statement : "for" "(" expression? ";" expression? ";" expression? ")" statement')
@glrp.rule('iteration-statement : "for" "(" declaration expression? ";" expression? ")" statement')
def iteration_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('jump-statement : "goto" "identifier" ";"')
@glrp.rule('jump-statement : "continue" ";"')
@glrp.rule('jump-statement : "break" ";"')
@glrp.rule('jump-statement : "return" expression? ";"')
def jump_statement(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ..parser import CParser