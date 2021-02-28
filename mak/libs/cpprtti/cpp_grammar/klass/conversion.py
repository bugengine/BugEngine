"""
conversion-function-id:
      operator conversion-type-id
conversion-type-id:
      type-specifier-seq conversion-declaratoropt
conversion-declarator:
      ptr-operator conversion-declaratoropt
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_conversion_function_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        conversion-function-id : KW_OPERATOR conversion-type-id
    """


@cpp98
def p_conversion_type_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        conversion-type-id : type-specifier-seq conversion-declarator?
    """


@cpp98
def p_conversion_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        conversion-declarator : ptr-operator conversion-declarator?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser