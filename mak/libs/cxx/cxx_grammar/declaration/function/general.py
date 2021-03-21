"""
function-definition:
  	attribute-specifier-seqopt decl-specifier-seqopt declarator function-body     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = default ;     C++0x
  	attribute-specifier-seqopt decl-specifier-seqopt declarator = delete ;     C++0x
function-body:
  	ctor-initializeropt compound-statement     C++0x
  	function-try-block     C++0x
"""

from ....cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("function-definition : attribute-specifier-seq? decl-specifier-seq? declarator function-body")
@cxx98
def p_function_definition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("function-definition : attribute-specifier-seq? decl-specifier-seq? declarator '=' 'default' ';'")
@glrp.rule("function-definition : attribute-specifier-seq? decl-specifier-seq? declarator '=' 'delete' ';'")
@cxx11
def p_function_definition_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("function-body : ctor-initializer? compound-statement | function-try-block")
@cxx98
def p_function_body(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser