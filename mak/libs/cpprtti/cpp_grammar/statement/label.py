"""
labeled-statement:
      attribute-specifier-seqopt identifier : statement
      attribute-specifier-seqopt case constant-expression : statement
      attribute-specifier-seqopt default: statement
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_labeled_statement(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        labeled-statement : attribute-specifier-seq? IDENTIFIER COLON statement
                          | attribute-specifier-seq? KW_CASE constant-expression COLON statement
                          | attribute-specifier-seq? KW_DEFAULT COLON statement
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser