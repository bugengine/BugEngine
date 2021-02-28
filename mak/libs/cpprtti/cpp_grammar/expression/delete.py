"""
delete-expression:
      ::opt delete cast-expression
      ::opt delete [ ] cast-expression
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_delete_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        delete-expression : OP_SCOPE KW_DELETE cast-expression
                          | OP_SCOPE KW_DELETE LBRACKET RBRACKET cast-expression
                          | KW_DELETE cast-expression
                          | KW_DELETE LBRACKET RBRACKET cast-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser