"""
labeled-statement:
      attribute-specifier-seqopt identifier : statement
      attribute-specifier-seqopt case constant-expression : statement
      attribute-specifier-seqopt default: statement
"""

from be_typing import TYPE_CHECKING


def p_labeled_statement(p):
    # type: (YaccProduction) -> None
    """
        labeled-statement : attribute-specifier-seq? identifier COLON statement
                          | attribute-specifier-seq? KW_CASE constant-expression COLON statement
                          | attribute-specifier-seq? KW_DEFAULT COLON statement
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction