from . import basic
from . import expression
from . import statement
from . import declaration
from . import namespace
from . import klass
from . import overload
from . import template
from . import exception
from . import bugengine_extensions
from ..cpp_parser import cpp98

from be_typing import TYPE_CHECKING


@cpp98
def p_empty(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        empty : %prec EMPTY
    """
    p[0] = None


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ..cpp_parser import CppParser