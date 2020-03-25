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
        elaborated-type-specifier : class-key attribute-specifier-seq-opt OP_SCOPE nested-name-specifier-opt identifier
                                  | class-key attribute-specifier-seq-opt nested-name-specifier-opt identifier
                                  | class-key OP_SCOPE nested-name-specifier-opt KW_TEMPLATE simple-template-id
                                  | class-key nested-name-specifier-opt KW_TEMPLATE simple-template-id
                                  | class-key OP_SCOPE nested-name-specifier-opt simple-template-id
                                  | class-key nested-name-specifier-opt simple-template-id
                                  | enum-key attribute-specifier-seq-opt OP_SCOPE nested-name-specifier-opt identifier
                                  | enum-key attribute-specifier-seq-opt nested-name-specifier-opt identifier
    """
    # case 2/3/4/5/6/7 should not have attribute-specifier-seq-opt


if TYPE_CHECKING:
    from ply.yacc import YaccProduction