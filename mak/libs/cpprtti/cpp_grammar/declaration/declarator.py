"""
init-declarator-list:
      init-declarator
      init-declarator-list , init-declarator
init-declarator:
      declarator initializeropt
declarator:
      ptr-declarator     C++0x
      noptr-declarator parameters-and-qualifiers trailing-return-type     C++0x
ptr-declarator:
      noptr-declarator     C++0x
      ptr-operator ptr-declarator     C++0x
noptr-declarator:
      declarator-id attribute-specifier-seqopt     C++0x
      noptr-declarator parameters-and-qualifiers     C++0x
      noptr-declarator [ constant-expressionopt ] attribute-specifier-seqopt     C++0x
      ( ptr-declarator )     C++0x
parameters-and-qualifiers:
      ( parameter-declaration-clause ) attribute-specifier-seqopt cv-qualifier-seqopt ref-qualifieropt exception-specificationopt     C++0x
trailing-return-type:
      -> trailing-type-specifier-seq abstract-declaratoropt     C++0x
ptr-operator:
      * attribute-specifier-seqopt cv-qualifier-seqopt     C++0x
      & attribute-specifier-seqopt     C++0x
      && attribute-specifier-seqopt     C++0x
      ::opt nested-name-specifier * attribute-specifier-seqopt cv-qualifier-seqopt     C++0x
cv-qualifier-seq:
      cv-qualifier
      cv-qualifier cv-qualifier-seq
cv-qualifier:
      const
      volatile
ref-qualifier:
      &     C++0x
      &&     C++0x
declarator-id:
      ...opt id-expression     C++0x
      ::opt nested-name-specifieropt class-name     C++0x

"""

from be_typing import TYPE_CHECKING


def p_init_declarator_list(p):
    # type: (YaccProduction) -> None
    """
        init-declarator-list : init-declarator
                             | init-declarator-list COMMA init-declarator
    """


def p_init_declarator(p):
    # type: (YaccProduction) -> None
    """
        init-declarator : declarator initializer?
    """


def p_declarator(p):
    # type: (YaccProduction) -> None
    """
        declarator : ptr-declarator
                   | noptr-declarator parameters-and-qualifiers trailing-return-type
    """


def p_ptr_declarator(p):
    # type: (YaccProduction) -> None
    """
        ptr-declarator : noptr-declarator
                       | ptr-operator ptr-declarator
    """


def p_noptr_declarator(p):
    # type: (YaccProduction) -> None
    """
        noptr-declarator : declarator-id attribute-specifier-seq?
                         | noptr-declarator parameters-and-qualifiers
                         | noptr-declarator LBRACKET constant-expression? RBRACKET attribute-specifier-seq?
                         | LPAREN ptr-declarator RPAREN
    """


def p_parameters_and_qualifiers(p):
    # type: (YaccProduction) -> None
    """
        parameters-and-qualifiers : LPAREN parameter-declaration-clause RPAREN attribute-specifier-seq? cv-qualifier-seq? ref-qualifier? exception-specification?
    """


def p_trailing_return_type(p):
    # type: (YaccProduction) -> None
    """
        trailing-return-type : OP_ARROW trailing-type-specifier-seq abstract-declarator?
    """


def p_ptr_operator(p):
    # type: (YaccProduction) -> None
    """
        ptr-operator : OP_TIMES attribute-specifier-seq? cv-qualifier-seq?
                     | OP_AND attribute-specifier-seq?
                     | OP_LAND attribute-specifier-seq?
                     | OP_SCOPE? nested-name-specifier OP_TIMES attribute-specifier-seq? cv-qualifier-seq?
    """


def p_cv_qualifier_seq(p):
    # type: (YaccProduction) -> None
    """
        cv-qualifier-seq : cv-qualifier
                         | cv-qualifier cv-qualifier-seq
    """


def p_cv_qualifier(p):
    # type: (YaccProduction) -> None
    """
        cv-qualifier : KW_CONST
                     | KW_VOLATILE
    """


def p_ref_qualifier(p):
    # type: (YaccProduction) -> None
    """
        ref-qualifier : OP_AND
                      | OP_LAND
    """


def p_declarator_id(p):
    # type: (YaccProduction) -> None
    """
        declarator-id : ELLIPSIS? id-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction