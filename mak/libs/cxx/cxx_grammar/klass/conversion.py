"""
conversion-function-id:
      operator conversion-type-id
conversion-type-id:
      type-specifier-seq conversion-declaratoropt
conversion-declarator:
      ptr-operator conversion-declaratoropt
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("conversion-function-id : 'operator' conversion-type-id")
@cxx98
def p_conversion_function_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("conversion-type-id : type-specifier-seq [prec:left,1] conversion-declarator?")
@cxx98
def p_conversion_type_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("conversion-declarator : ptr-operator [prec:left,1] conversion-declarator?")
@cxx98
def p_conversion_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser