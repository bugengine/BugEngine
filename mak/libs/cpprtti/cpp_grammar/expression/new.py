"""
new-expression:
      ::opt new new-placementopt new-type-id new-initializeropt
      ::opt new new-placementopt ( type-id ) new-initializeropt
new-placement:
      ( expression-list )
new-type-id:
      type-specifier-seq new-declaratoropt
new-declarator:
      ptr-operator new-declaratoropt
      noptr-new-declarator     C++0x
noptr-new-declarator:
      [ expression ] attribute-specifier-seqopt     C++0x
      noptr-new-declarator [ constant-expression ] attribute-specifier-seqopt     C++0x
new-initializer:
      ( expression-listopt )
      braced-init-list     C++0x
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_new_expression(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        new-expression : OP_SCOPE KW_NEW new-placement? new-type-id new-initializer?
                       | OP_SCOPE KW_NEW new-placement? LPAREN type-id RPAREN new-initializer?
                       | KW_NEW new-placement? new-type-id new-initializer?
                       | KW_NEW new-placement? LPAREN type-id RPAREN new-initializer?
    """


@cpp98
def p_new_placement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        new-placement : LPAREN expression-list RPAREN
    """


@cpp98
def p_new_type_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        new-type-id : type-specifier-seq new-declarator?
    """


@cpp98
def p_new_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        new-declarator : ptr-operator new-declarator?
                       | noptr-new-declarator
    """


@cpp98
def p_noptr_new_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-new-declarator : LBRACKET expression RBRACKET attribute-specifier-seq?
                             | noptr-new-declarator LBRACKET constant-expression RBRACKET attribute-specifier-seq?
    """


@cpp98
def p_new_initializer(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        new-initializer : LPAREN expression-list? RPAREN
                        | braced-init-list
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser