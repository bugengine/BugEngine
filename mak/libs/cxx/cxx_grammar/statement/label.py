"""
labeled-statement:
      attribute-specifier-seqopt identifier : statement
      attribute-specifier-seqopt case constant-expression : statement
      attribute-specifier-seqopt default: statement
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("labeled-statement : attribute-specifier-seq? 'identifier' ':' statement")
@glrp.rule("labeled-statement : attribute-specifier-seq? 'case' constant-expression ':' statement")
@glrp.rule("labeled-statement : attribute-specifier-seq? 'default' ':' statement")
@cxx98
def p_labeled_statement(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser