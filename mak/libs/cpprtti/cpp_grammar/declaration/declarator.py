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

from ...cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@cpp98
def p_init_declarator_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        init-declarator-list : init-declarator
                             | init-declarator-list COMMA init-declarator
    """


@cpp98
def p_init_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        init-declarator : declarator initializer?
    """


@cpp98
def p_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declarator : ptr-declarator
    """


@cpp11
def p_declarator_trailing(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declarator : noptr-declarator parameters-and-qualifiers trailing-return-type
    """


@cpp98
def p_ptr_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ptr-declarator : noptr-declarator
                       | ptr-operator ptr-declarator
    """


@cpp98
def p_noptr_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-declarator : declarator-id attribute-specifier-seq?
                         | noptr-declarator parameters-and-qualifiers
                         | noptr-declarator LBRACKET constant-expression? RBRACKET attribute-specifier-seq?
                         | LPAREN ptr-declarator RPAREN
    """


@cpp98
def p_noptr_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-declarator : declarator-id attribute-specifier-seq?
                         | noptr-declarator parameters-and-qualifiers
                         | noptr-declarator LBRACKET  RBRACKET attribute-specifier-seq?
                         | LPAREN ptr-declarator RPAREN
    """


@cpp98
def p_parameters_and_qualifiers(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        parameters-and-qualifiers : LPAREN parameter-declaration-clause RPAREN attribute-specifier-seq? cv-qualifier-seq? ref-qualifier exception-specification?
    """


@cpp11
def p_trailing_return_type(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        trailing-return-type : OP_ARROW trailing-type-specifier-seq abstract-declarator?
    """


@cpp98
def p_ptr_operator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ptr-operator : OP_TIMES attribute-specifier-seq? cv-qualifier-seq?
                     | OP_AND attribute-specifier-seq? cv-qualifier-seq?
                     | OP_SCOPE nested-name-specifier OP_TIMES attribute-specifier-seq? cv-qualifier-seq? %prec SCOPE_REDUCTION
                     | nested-name-specifier OP_TIMES attribute-specifier-seq? cv-qualifier-seq?
    """


@cpp11
def p_ptr_operator_rvalue_ref(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ptr-operator : OP_LAND attribute-specifier-seq? cv-qualifier-seq?
    """


@cpp98
def p_cv_qualifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        cv-qualifier-seq : cv-qualifier
                         | cv-qualifier cv-qualifier-seq
    """


@cpp98
def p_cv_qualifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        cv-qualifier : KW_CONST
                     | KW_VOLATILE
    """


@cpp98
def p_ref_qualifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ref-qualifier : empty
    """


@cpp11
def p_ref_qualifier_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ref-qualifier : OP_AND
                      | OP_LAND
    """


@cpp98
def p_declarator_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declarator-id : id-expression
    """


@cpp11
def p_declarator_id_ellipsis(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declarator-id : ELLIPSIS id-expression
    """


@disabled
def p_declarator_id_disabled(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        declarator-id : OP_SCOPE? nested-name-specifier? class-name
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser