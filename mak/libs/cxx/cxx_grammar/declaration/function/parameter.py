"""
parameter-declaration-clause:
  	parameter-declaration-listopt ...opt
  	parameter-declaration-list , ...
parameter-declaration-list:
  	parameter-declaration
  	parameter-declaration-list , parameter-declaration
parameter-declaration:
  	attribute-specifier-seqopt decl-specifier-seq declarator     C++0x
  	attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause     C++0x
  	attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt     C++0x
  	attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt = initializer-clause     C++0x
"""

from ....cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("parameter-declaration-clause : parameter-declaration-list?")
@glrp.rule("parameter-declaration-clause : parameter-declaration-list ',' '...'")
@cxx98
def p_parameter_declaration_clause(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("parameter-declaration-clause : parameter-declaration-list? '...'")
@cxx11
def p_parameter_declaration_clause_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    # TODO: C++11?
    pass


@glrp.rule("parameter-declaration-clause : parameter-declaration-list '...'")
@cxx11
def p_parameter_declaration_clause_texpand(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("parameter-declaration-list : parameter-declaration")
@glrp.rule("parameter-declaration-list : parameter-declaration-list ',' parameter-declaration")
@cxx98
def p_parameter_declaration_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator")
@glrp.rule("parameter-declaration : attribute-specifier-seq? decl-specifier-seq declarator '=' initializer-clause")
@glrp.rule("parameter-declaration : attribute-specifier-seq? decl-specifier-seq abstract-declarator?")
@glrp.rule(
    "parameter-declaration : attribute-specifier-seq? decl-specifier-seq abstract-declarator? '=' initializer-clause"
)
@cxx98
def p_parameter_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser