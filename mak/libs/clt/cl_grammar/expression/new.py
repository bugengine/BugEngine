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

from be_typing import TYPE_CHECKING


def p_new_expression(p):
    # type: (YaccProduction) -> None
    """
        new-expression : OP_SCOPE KW_NEW new-placement-opt new-type-id new-initializer-opt
                       | KW_NEW new-placement-opt new-type-id new-initializer-opt
                       | OP_SCOPE KW_NEW new-placement-opt LPAREN type-id RPAREN new-initializer-opt
                       | KW_NEW new-placement-opt LPAREN type-id RPAREN new-initializer-opt
    """


def p_new_placement(p):
    # type: (YaccProduction) -> None
    """
        new-placement : LPAREN expression-list RPAREN
    """


def p_new_placement_opt(p):
    # type: (YaccProduction) -> None
    """
        new-placement-opt : new-placement
    """
    # TODO: empty


def p_new_type_id(p):
    # type: (YaccProduction) -> None
    """
        new-type-id : type-specifier-seq new-declarator-opt
    """


def p_new_declarator(p):
    # type: (YaccProduction) -> None
    """
        new-declarator : ptr-operator new-declarator-opt
                       | noptr-new-declarator
    """


def p_new_declarator_opt(p):
    # type: (YaccProduction) -> None
    """
        new-declarator-opt : new-declarator
    """
    # TODO: empty


def p_noptr_new_declarator(p):
    # type: (YaccProduction) -> None
    """
        noptr-new-declarator : LBRACKET expression RBRACKET attribute-specifier-seq-opt
                             | noptr-new-declarator LBRACKET constant-expression RBRACKET attribute-specifier-seq-opt
    """


def p_new_initializer(p):
    # type: (YaccProduction) -> None
    """
        new-initializer : LPAREN expression-list-opt RPAREN
                        | braced-init-list
    """


def p_new_initializer_opt(p):
    # type: (YaccProduction) -> None
    """
        new-initializer-opt : new-initializer
    """
    # TODO: empty


if TYPE_CHECKING:
    from ply.yacc import YaccProduction