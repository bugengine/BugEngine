"""
type-specifier:
      trailing-type-specifier
      class-specifier
      enum-specifier
trailing-type-specifier:
      simple-type-specifier
      elaborated-type-specifier
      typename-specifier
      cv-qualifier
type-specifier-seq:
      type-specifier attribute-specifier-seqopt     C++0x
      type-specifier type-specifier-seq
trailing-type-specifier-seq:
      trailing-type-specifier attribute-specifier-seqopt     C++0x
      trailing-type-specifier trailing-type-specifier-seq     C++0x
"""

from ....cxx_parser import cxx98, cxx11
from . import simple
from . import elaborate
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("type-specifier : trailing-type-specifier | class-specifier | enum-specifier")
@cxx98
def p_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("trailing-type-specifier : simple-type-specifier")
@glrp.rule("trailing-type-specifier : elaborated-type-specifier")
@glrp.rule("trailing-type-specifier : typename-specifier")
@glrp.rule("trailing-type-specifier : cv-qualifier")
@cxx98
def p_trailing_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("type-specifier-seq : type-specifier attribute-specifier-seq?")
@glrp.rule("type-specifier-seq : type-specifier type-specifier-seq")
@cxx98
def p_type_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("trailing-type-specifier-seq : trailing-type-specifier attribute-specifier-seq?")
@glrp.rule("trailing-type-specifier-seq : trailing-type-specifier trailing-type-specifier-seq")
@cxx11
def p_trailing_type_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser