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

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("new-expression : '::'? 'new' new-placement? new-type-id new-initializer?")
@glrp.rule("new-expression : '::'? 'new' new-placement? '(' type-id ')' new-initializer?")
@cxx98
def p_new_expression(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("new-placement : '(' expression-list ')'")
@cxx98
def p_new_placement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("new-type-id : type-specifier-seq new-declarator?")
@cxx98
def p_new_type_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("new-declarator : ptr-operator new-declarator?")
@glrp.rule("new-declarator : noptr-new-declarator")
@cxx98
def p_new_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("noptr-new-declarator : '[' expression ']' attribute-specifier-seq?")
@glrp.rule("noptr-new-declarator : noptr-new-declarator '[' constant-expression ']' attribute-specifier-seq?")
@cxx98
def p_noptr_new_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("new-initializer : '(' expression-list? ')'")
@cxx98
def p_new_initializer(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("new-initializer : braced-init-list")
@cxx11
def p_new_initializer_braced_init_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser