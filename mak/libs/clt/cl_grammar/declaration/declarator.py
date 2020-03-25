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


def p_init_declarator_list_opt(p):
    # type: (YaccProduction) -> None
    """
        init-declarator-list-opt : init-declarator-list
                                 | empty
    """
    # TODO: empty


def p_init_declarator(p):
    # type: (YaccProduction) -> None
    """
        init-declarator : declarator initializer-opt
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
        noptr-declarator : declarator-id attribute-specifier-seq-opt
                         | noptr-declarator parameters-and-qualifiers
                         | noptr-declarator LBRACKET constant-expression-opt RBRACKET attribute-specifier-seq-opt
                         | LPAREN ptr-declarator RPAREN
    """


def p_parameters_and_qualifiers(p):
    # type: (YaccProduction) -> None
    """
        parameters-and-qualifiers : LPAREN parameter-declaration-clause RPAREN attribute-specifier-seq-opt cv-qualifier-seq-opt ref-qualifier-opt exception-specification-opt
    """


def p_trailing_return_type(p):
    # type: (YaccProduction) -> None
    """
        trailing-return-type : OP_ARROW trailing-type-specifier-seq abstract-declarator-opt
    """


def p_trailing_return_type_opt(p):
    # type: (YaccProduction) -> None
    """
        trailing-return-type-opt : trailing-return-type
    """
    # TODO: empty


def p_ptr_operator(p):
    # type: (YaccProduction) -> None
    """
        ptr-operator : OP_TIMES attribute-specifier-seq-opt cv-qualifier-seq-opt
                     | OP_AND attribute-specifier-seq-opt
                     | OP_LAND attribute-specifier-seq-opt
                     | OP_SCOPE nested-name-specifier OP_TIMES attribute-specifier-seq-opt cv-qualifier-seq-opt
                     | nested-name-specifier OP_TIMES attribute-specifier-seq-opt cv-qualifier-seq-opt
    """


def p_cv_qualifier_seq(p):
    # type: (YaccProduction) -> None
    """
        cv-qualifier-seq : cv-qualifier
                         | cv-qualifier cv-qualifier-seq
    """


def p_cv_qualifier_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        cv-qualifier-seq-opt : cv-qualifier-seq
                             | empty
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


def p_ref_qualifier_opt(p):
    # type: (YaccProduction) -> None
    """
        ref-qualifier-opt : ref-qualifier
    """
    # TODO: empty


def p_declarator_id(p):
    # type: (YaccProduction) -> None
    """
        declarator-id : ellipsis-opt id-expression
                      | OP_SCOPE nested-name-specifier-opt class-name
                      | nested-name-specifier class-name
                      | class-name
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction