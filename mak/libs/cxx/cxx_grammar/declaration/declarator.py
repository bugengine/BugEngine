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

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("init-declarator-list : init-declarator | init-declarator-list ',' init-declarator")
@cxx98
def p_init_declarator_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("init-declarator : declarator initializer?")
@cxx98
def p_init_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("declarator : ptr-declarator")
@cxx98
def p_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("declarator : noptr-declarator parameters-and-qualifiers trailing-return-type")
@cxx11
def p_declarator_trailing(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ptr-declarator : noptr-declarator | ptr-operator ptr-declarator")
@cxx98
def p_ptr_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("noptr-declarator : declarator-id attribute-specifier-seq?")
@glrp.rule("noptr-declarator : noptr-declarator parameters-and-qualifiers")
@glrp.rule("noptr-declarator : noptr-declarator '[' constant-expression? ']' attribute-specifier-seq?")
@glrp.rule("noptr-declarator : '(' ptr-declarator ')'")
@cxx98
def p_noptr_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule(
    "parameters-and-qualifiers : '(' parameter-declaration-clause ')' attribute-specifier-seq? cv-qualifier-seq? ref-qualifier exception-specification?"
)
@cxx98
def p_parameters_and_qualifiers(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("trailing-return-type : '->' trailing-type-specifier-seq abstract-declarator?")
@cxx11
def p_trailing_return_type(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ptr-operator : '*' attribute-specifier-seq? cv-qualifier-seq?")
@glrp.rule("ptr-operator : '&' attribute-specifier-seq?")
@glrp.rule("ptr-operator : '::' nested-name-specifier '*' attribute-specifier-seq? cv-qualifier-seq?")
@glrp.rule("ptr-operator : nested-name-specifier '*' attribute-specifier-seq? cv-qualifier-seq?")
@cxx98
def p_ptr_operator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ptr-operator : '&&' attribute-specifier-seq?")
@cxx11
def p_ptr_operator_rvalue_ref(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("cv-qualifier-seq : cv-qualifier | cv-qualifier cv-qualifier-seq")
@cxx98
def p_cv_qualifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("cv-qualifier : 'const' | 'volatile'")
@cxx98
def p_cv_qualifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ref-qualifier :")
@cxx98
def p_ref_qualifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ref-qualifier : '&' | '&&'")
@cxx11
def p_ref_qualifier_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("declarator-id : id-expression | id-type")
@cxx98
def p_declarator_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("declarator-id : '...' id-expression | '...' id-type")
@cxx11
def p_declarator_id_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser