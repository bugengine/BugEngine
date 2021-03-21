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

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("type-id : type-specifier-seq abstract-declarator?")
@cxx98
def p_type_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("abstract-declarator : ptr-abstract-declarator")
@cxx98
def p_abstract_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("abstract-declarator : '...'")
@cxx11
def p_abstract_declarator_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: really necessary?


@glrp.rule("abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers trailing-return-type")
@cxx11
def p_abstract_declarator_trailing_type(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("ptr-abstract-declarator : noptr-abstract-declarator")
@glrp.rule("ptr-abstract-declarator : ptr-operator ptr-abstract-declarator?")
@cxx98
def p_ptr_abstract_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("noptr-abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers")
@glrp.rule(
    "noptr-abstract-declarator : noptr-abstract-declarator? '[' constant-expression ']' attribute-specifier-seq?"
)
@glrp.rule("noptr-abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers")
@cxx98
def p_noptr_abstract_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cxx_parser import CxxParser