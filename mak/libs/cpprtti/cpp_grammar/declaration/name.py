"""
type-id:
      type-specifier-seq abstract-declaratoropt
abstract-declarator:
      ptr-abstract-declarator     C++0x
      noptr-abstract-declaratoropt parameters-and-qualifiers trailing-return-type     C++0x
      ...     C++0x
ptr-abstract-declarator:
      noptr-abstract-declarator     C++0x
      ptr-operator ptr-abstract-declaratoropt     C++0x
noptr-abstract-declarator:
      noptr-abstract-declaratoropt parameters-and-qualifiers     C++0x
      noptr-abstract-declaratoropt [ constant-expression ] attribute-specifier-seqopt     C++0x
      ( ptr-abstract-declarator )     C++0x
"""

from be_typing import TYPE_CHECKING


def p_type_id(p):
    # type: (YaccProduction) -> None
    """
        type-id : type-specifier-seq abstract-declarator-opt
    """


def p_abstract_declarator(p):
    # type: (YaccProduction) -> None
    """
        abstract-declarator : ptr-abstract-declarator
                            | noptr-abstract-declarator parameters-and-qualifiers trailing-return-type
                            | parameters-and-qualifiers trailing-return-type
                            | ELLIPSIS
    """


def p_abstract_declarator_opt(p):
    # type: (YaccProduction) -> None
    """
        abstract-declarator-opt : abstract-declarator
                                | empty
    """


def p_ptr_abstract_declarator(p):
    # type: (YaccProduction) -> None
    """
        ptr-abstract-declarator : noptr-abstract-declarator
                                | ptr-operator ptr-abstract-declarator
                                | ptr-operator
    """


def p_noptr_abstract_declarator(p):
    # type: (YaccProduction) -> None
    """
        noptr-abstract-declarator : noptr-abstract-declarator parameters-and-qualifiers
                                  | noptr-abstract-declarator LBRACKET constant-expression RBRACKET attribute-specifier-seq-opt
                                  | parameters-and-qualifiers
                                  | LBRACKET constant-expression RBRACKET attribute-specifier-seq-opt
                                  | LPAREN ptr-abstract-declarator RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction