"""
elaborated-type-specifier:
      class-key attribute-specifier-seqopt ::opt nested-name-specifieropt identifier
      class-key ::opt nested-name-specifieropt templateopt simple-template-id
      enum ::opt nested-name-specifieropt identifier
"""

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_elaborate_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        elaborated-type-specifier : class-key attribute-specifier-seq? id-expression
                                  | enum-key id-expression
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser