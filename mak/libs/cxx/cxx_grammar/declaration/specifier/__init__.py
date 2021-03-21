"""
decl-specifier:
  	storage-class-specifier
  	type-specifier
  	function-specifier
  	friend
  	typedef
  	constexpr     C++0x
decl-specifier-seq:
  	decl-specifier attribute-specifier-seqopt     C++0x
  	decl-specifier decl-specifier-seq     C++0x
"""

from ....cxx_parser import cxx98, cxx11, cxx20
from . import storage_class
from . import function
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("decl-specifier : storage-class-specifier | type-specifier | function-specifier | 'friend' | 'typedef'")
@cxx98
def p_decl_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("decl-specifier : 'constexpr'")
@cxx11
def p_decl_specifier_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("decl-specifier : 'consteval' | 'constinit'")
@cxx20
def p_decl_specifier_cxx20(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("decl-specifier-seq : decl-specifier attribute-specifier-seq?")
@glrp.rule("decl-specifier-seq : decl-specifier decl-specifier-seq")
@cxx98
def p_decl_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser