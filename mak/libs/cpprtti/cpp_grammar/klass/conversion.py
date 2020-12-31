"""
conversion-function-id:
      operator conversion-type-id
conversion-type-id:
      type-specifier-seq conversion-declaratoropt
conversion-declarator:
      ptr-operator conversion-declaratoropt
"""

from be_typing import TYPE_CHECKING


def p_conversion_function_id(p):
    # type: (YaccProduction) -> None
    """
        conversion-function-id : KW_OPERATOR conversion-type-id
    """


def p_conversion_type_id(p):
    # type: (YaccProduction) -> None
    """
        conversion-type-id : type-specifier-seq conversion-declarator-opt
    """


def p_conversion_declarator(p):
    # type: (YaccProduction) -> None
    """
        conversion-declarator : ptr-operator conversion-declarator-opt
    """


def p_conversion_declarator_opt(p):
    # type: (YaccProduction) -> None
    """
        conversion-declarator-opt : conversion-declarator
                                  | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction