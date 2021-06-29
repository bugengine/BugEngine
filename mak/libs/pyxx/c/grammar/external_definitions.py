"""
A.2.4 External definitions
(6.9) translation-unit:
    external-declaration
    translation-unit external-declaration
(6.9) external-declaration:
    function-definition
    declaration
(6.9.1) function-definition:
    attribute-specifier-sequenceopt declaration-specifiers declarator function-body
(6.9.1) function-body:
    compound-statement
"""

import glrp
from ..parser import c89
from be_typing import TYPE_CHECKING


@c89
@glrp.rule("translation-unit : external-declaration")
@glrp.rule("translation-unit : translation-unit external-declaration")
def translation_unit(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule("external-declaration : function-definition")
@glrp.rule("external-declaration : declaration")
def external_declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule("function-definition : attribute-specifier-sequence? declaration-specifiers declarator function-body")
def function_definition(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule("function-body : compound-statement")
def function_body(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ..parser import CParser