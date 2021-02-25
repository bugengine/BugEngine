"""
template-declaration:
      template < template-parameter-list > declaration     C++0x - The export keyword is reserved for future use
template-parameter-list:
      template-parameter
      template-parameter-list , template-parameter
"""

from .parameter import *
from .names import *
from .typename import *
from .explicit import *

from be_typing import TYPE_CHECKING


def p_template_declaration(p):
    # type: (YaccProduction) -> None
    """
        template-declaration : attribute-specifier-seq? decl-specifier-seq? KW_TEMPLATE LANGLE template-parameter-list RANGLE declaration
    """
    # TODO: attributes empty
    # TODO: specifier empty


def p_template_parameter_list(p):
    # type: (YaccProduction) -> None
    """
        template-parameter-list : template-parameter
                                | template-parameter-list COMMA template-parameter
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction