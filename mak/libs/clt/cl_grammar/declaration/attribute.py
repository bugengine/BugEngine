"""
attribute-specifier-seq:
      attribute-specifier     C++0x
      attribute-specifier-seq attribute-specifier     C++0x
attribute-specifier:
      [ [ attribute-list ] ]     C++0x
      alignment-specifier     C++0x
alignment-specifier:
      alignas ( type-id ...opt )     C++0x
      alignas ( alignment-expression ...opt )     C++0x
attribute-list:
      attributeopt     C++0x
      attribute-list , attributeopt     C++0x
      attribute ...     C++0x
      attribute-list , attribute ...     C++0x
attribute:
      attribute-token attribute-argument-clauseopt     C++0x
attribute-token:
      identifier     C++0x
      attribute-scoped-token     C++0x
attribute-scoped-token:
      attribute-namespace :: identifier     C++0x
attribute-namespace:
      identifier     C++0x
attribute-argument-clause: 	 
      ( balanced-token-seq )     C++0x
balanced-token-seq:
      balanced-token     C++0x
      balanced-token-seq balanced-token     C++0x
balanced-token:
      ( balanced-token-seq )     C++0x
      [ balanced-token-seq ]     C++0x
      { balanced-token-seq }     C++0x
      token     C++0x - except a parenthesis, a bracket, or a brace
"""

from be_typing import TYPE_CHECKING


def p_attribute_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        attribute-specifier-seq : attribute-specifier
                                | attribute-specifier-seq attribute-specifier
    """


def p_attribute_specifier_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        attribute-specifier-seq-opt : attribute-specifier-seq
    """
    # TODO: empty


def p_attribute_specifier(p):
    # type: (YaccProduction) -> None
    """
        attribute-specifier : LBRACKET LBRACKET attribute-list RBRACKET RBRACKET
                            | alignment-specifier
    """


def p_alignment_specifier(p):
    # type: (YaccProduction) -> None
    """
        alignment-specifier : KW_ALIGNAS LPAREN type-id ellipsis-opt RPAREN
                            | KW_ALIGNAS LPAREN constant-expression ellipsis-opt RPAREN
    """


def p_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        attribute-list : attribute-opt
                       | attribute-list COMMA attribute-opt
                       | attribute ELLIPSIS
                       | attribute-list COMMA attribute ELLIPSIS
    """


def p_attribute(p):
    # type: (YaccProduction) -> None
    """
        attribute : attribute-token attribute-argument-clause
                  | attribute-token
    """


def p_attribute_opt(p):
    # type: (YaccProduction) -> None
    """
        attribute-opt : attribute
                      | empty
    """


def p_attribute_token(p):
    # type: (YaccProduction) -> None
    """
        attribute-token : identifier
                        | attribute-scoped-token
    """


def p_attribute_scoped_token(p):
    # type: (YaccProduction) -> None
    """
        attribute-scoped-token : attribute-namespace OP_SCOPE identifier
    """


def p_attribute_namespace(p):
    # type: (YaccProduction) -> None
    """
        attribute-namespace : identifier
    """


def p_attribute_argument_clause(p):
    # type: (YaccProduction) -> None
    """
        attribute-argument-clause : LPAREN balanced-token-seq RPAREN
    """


def p_attribute_argument_clause_opt(p):
    # type: (YaccProduction) -> None
    """
        attribute-argument-clause-opt : attribute-argument-clause
                                      | empty
    """
    # TODO: empty


def p_balanced_token_seq(p):
    # type: (YaccProduction) -> None
    """
        balanced-token-seq : balanced-token
                           | balanced-token-seq balanced-token
                           | empty
    """


def p_balanced_token(p):
    # type: (YaccProduction) -> None
    """
        balanced-token : attribute-argument-clause
                       | LBRACKET balanced-token-seq RBRACKET
                       | LBRACE balanced-token-seq RBRACE
                       | KW_AUTO
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction