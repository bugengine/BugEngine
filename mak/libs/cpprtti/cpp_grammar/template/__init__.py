"""
template-declaration:
      template < template-parameter-list > declaration     C++0x - The export keyword is reserved for future use
template-parameter-list:
      template-parameter
      template-parameter-list , template-parameter
"""

from . import parameter
from . import names
from . import typename
from . import explicit

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_template_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-declaration : attribute-specifier-seq? decl-specifier-seq? KW_TEMPLATE LANGLE template-parameter-list RANGLE declaration
    """
    # TODO: attributes empty
    # TODO: specifier empty
    # TODO: storage class empty


@cpp98
def p_template_parameter_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        template-parameter-list : template-parameter
                                | template-parameter-list COMMA template-parameter
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser