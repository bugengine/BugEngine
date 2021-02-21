"""
elaborated-type-specifier:
      class-key attribute-specifier-seqopt ::opt nested-name-specifieropt identifier
      class-key ::opt nested-name-specifieropt templateopt simple-template-id
      enum ::opt nested-name-specifieropt identifier
"""

from be_typing import TYPE_CHECKING


def p_elaborate_type_specifier(p):
    # type: (YaccProduction) -> None
    """
        elaborated-type-specifier : class-key attribute-specifier-seq-opt scope-opt nested-name-specifier-opt identifier
                                  | class-key scope-opt nested-name-specifier-opt template-opt simple-template-id
                                  | enum-key scope-opt nested-name-specifier-opt identifier
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction