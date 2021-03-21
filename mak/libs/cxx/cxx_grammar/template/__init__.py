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

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule(
    "template-declaration : attribute-specifier-seq? decl-specifier-seq? 'template' '<[' template-parameter-list ']>' declaration"
)
@cxx98
def p_template_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # TODO: attributes empty
    # TODO: specifier empty
    # TODO: storage class empty


@glrp.rule("template-parameter-list : template-parameter | template-parameter-list ',' template-parameter")
@cxx98
def p_template_parameter_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        template-parameter-list : template-parameter
                                | template-parameter-list COMMA template-parameter
    """


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser