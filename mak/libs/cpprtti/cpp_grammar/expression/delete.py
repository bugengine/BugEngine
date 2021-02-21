"""
delete-expression:
      ::opt delete cast-expression
      ::opt delete [ ] cast-expression
"""

from be_typing import TYPE_CHECKING


def p_delete_expression(p):
    # type: (YaccProduction) -> None
    """
        delete-expression : OP_SCOPE? KW_DELETE cast-expression
                          | OP_SCOPE? KW_DELETE LBRACKET RBRACKET cast-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction